// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterPart.h"

#include "Character/DWCharacter.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Datas/VoxelData.h"

UDWCharacterPart::UDWCharacterPart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UPrimitiveComponent::SetCollisionProfileName(TEXT("CharacterPart"));
	InitBoxExtent(FVector(15, 15, 15));

	CharacterPartType = EDWCharacterPartType::None;
	LastOverlapVoxel = FVoxelItem();
}

void UDWCharacterPart::BeginPlay()
{
	Super::BeginPlay();
}

void UDWCharacterPart::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!GetOwnerCharacter() || !GetOwnerCharacter()->IsActive()) return;

	if(AVoxelChunk* Chunk = Cast<AVoxelChunk>(GetOwnerCharacter()->Execute_GetContainer(GetOwnerCharacter()).GetObject()))
	{
		FVoxelItem& VoxelItem = Chunk->GetVoxelItem(Chunk->LocationToIndex(GetComponentLocation()));
		const UVoxelData& VoxelData = VoxelItem.GetData<UVoxelData>();
		const FVoxelHitResult VoxelHitResult = FVoxelHitResult(VoxelItem, GetComponentLocation(), GetOwnerCharacter()->GetMoveDirection(false));
		if(VoxelItem.IsValid())
		{
			if(!LastOverlapVoxel.IsValid() || LastOverlapVoxel.Index != VoxelItem.Index)
			{
				if(LastOverlapVoxel.IsValid())
				{
					OnExitVoxel(LastOverlapVoxel.GetVoxel(), VoxelHitResult);
					LastOverlapVoxel = FVoxelItem();
				}
				LastOverlapVoxel = VoxelItem;
				VoxelItem.GetVoxel().OnTargetEnter(GetOwnerCharacter(), VoxelHitResult);
			}
			if(LastOverlapVoxel.IsValid())
			{
				OnStayVoxel(LastOverlapVoxel.GetVoxel(), VoxelHitResult);
			}
		}
		else if(LastOverlapVoxel.IsValid())
		{
			LastOverlapVoxel.GetVoxel().OnTargetExit(GetOwnerCharacter(), VoxelHitResult);
			LastOverlapVoxel = FVoxelItem();
		}
	}
}

void UDWCharacterPart::OnHitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel.OnTargetHit(GetOwnerCharacter(), InHitResult);
}

void UDWCharacterPart::OnEnterVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel.OnTargetEnter(GetOwnerCharacter(), InHitResult);

	UVoxelData& VoxelData = InVoxel.GetData();
	switch (VoxelData.VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case EDWCharacterPartType::Chest:
				{
					GetOwnerCharacter()->Swim();
					break;
				}
				case EDWCharacterPartType::Neck:
				{
					GetOwnerCharacter()->UnFloat();
					break;
				}
				default: break;
			}
			break;
		}
		default: break;
	}
}

void UDWCharacterPart::OnStayVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel.OnTargetStay(GetOwnerCharacter(), InHitResult);
}

void UDWCharacterPart::OnExitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel.OnTargetExit(GetOwnerCharacter(), InHitResult);

	UVoxelData& VoxelData = InVoxel.GetData();
	switch (VoxelData.VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case EDWCharacterPartType::Chest:
				{
					if(InHitResult.VoxelItem.GetData<UVoxelData>().VoxelType != EVoxelType::Water)
					{
						GetOwnerCharacter()->UnSwim();
					}
					break;
				}
				case EDWCharacterPartType::Neck:
				{
					if(InHitResult.VoxelItem.GetData<UVoxelData>().VoxelType != EVoxelType::Water)
					{
						GetOwnerCharacter()->Float(InVoxel.GetOwner()->IndexToLocation(InVoxel.GetIndex()).Z + UVoxelModuleBPLibrary::GetWorldData().BlockSize);
					}
					break;
				}
				default: break;
			}
			break;
		}
		default: break;
	}
}

ADWCharacter* UDWCharacterPart::GetOwnerCharacter() const
{
	return Cast<ADWCharacter>(GetOwner());
}
