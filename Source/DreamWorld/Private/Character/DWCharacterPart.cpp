// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterPart.h"

#include "Character/DWCharacter.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Voxels/VoxelAssetBase.h"

UDWCharacterPart::UDWCharacterPart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UPrimitiveComponent::SetCollisionProfileName(TEXT("DW_CharacterPart"));
	InitBoxExtent(FVector(15, 15, 15));

	CharacterPartType = ECharacterPartType::None;
	LastOverlapVoxel = nullptr;
}

void UDWCharacterPart::BeginPlay()
{
	Super::BeginPlay();
}

void UDWCharacterPart::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!GetOwnerCharacter() || !GetOwnerCharacter()->IsActive()) return;

	if(AVoxelChunk* Chunk = GetOwnerCharacter()->GetOwnerChunk())
	{
		const FVoxelItem& VoxelItem = Chunk->GetVoxelItem(Chunk->LocationToIndex(GetComponentLocation()));
		const UVoxelAssetBase* VoxelData = VoxelItem.GetData<UVoxelAssetBase>();
		const FVoxelHitResult VoxelHitResult = FVoxelHitResult(VoxelItem, GetComponentLocation(), GetOwnerCharacter()->GetMoveDirection(false));
		if(VoxelItem.IsValid())
		{
			if(!LastOverlapVoxel || LastOverlapVoxel->GetIndex() != VoxelItem.Index)
			{
				if(LastOverlapVoxel)
				{
					OnExitVoxel(LastOverlapVoxel, VoxelHitResult);
					LastOverlapVoxel->OnTargetExit(GetOwnerCharacter(), VoxelHitResult);
					UVoxel::DespawnVoxel(LastOverlapVoxel);
					LastOverlapVoxel = nullptr;
				}
				if(UVoxel* Voxel = VoxelItem.GetVoxel())
				{
					LastOverlapVoxel = Voxel;
					Voxel->OnTargetEnter(GetOwnerCharacter(), VoxelHitResult);
				}
			}
			if(LastOverlapVoxel)
			{
				OnStayVoxel(LastOverlapVoxel, VoxelHitResult);
			}
		}
		else if(LastOverlapVoxel)
		{
			LastOverlapVoxel->OnTargetExit(GetOwnerCharacter(), VoxelHitResult);
			UVoxel::DespawnVoxel(LastOverlapVoxel);
			LastOverlapVoxel = nullptr;
		}
	}
}

void UDWCharacterPart::OnHitVoxel(UVoxel* InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel->OnTargetHit(GetOwnerCharacter(), InHitResult);
}

void UDWCharacterPart::OnEnterVoxel(UVoxel* InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel->OnTargetEnter(GetOwnerCharacter(), InHitResult);

	UVoxelAssetBase* VoxelData = InVoxel->GetData();
	switch (VoxelData->VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case ECharacterPartType::Chest:
				{
					GetOwnerCharacter()->Swim();
					break;
				}
				case ECharacterPartType::Neck:
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

void UDWCharacterPart::OnStayVoxel(UVoxel* InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel->OnTargetStay(GetOwnerCharacter(), InHitResult);
}

void UDWCharacterPart::OnExitVoxel(UVoxel* InVoxel, const FVoxelHitResult& InHitResult)
{
	InVoxel->OnTargetExit(GetOwnerCharacter(), InHitResult);

	UVoxelAssetBase* VoxelData = InVoxel->GetData();
	switch (VoxelData->VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case ECharacterPartType::Chest:
				{
					if(InHitResult.VoxelItem.GetData<UVoxelAssetBase>()->VoxelType != EVoxelType::Water)
					{
						GetOwnerCharacter()->UnSwim();
					}
					break;
				}
				case ECharacterPartType::Neck:
				{
					if(InHitResult.VoxelItem.GetData<UVoxelAssetBase>()->VoxelType != EVoxelType::Water)
					{
						GetOwnerCharacter()->Float(InVoxel->GetOwner()->IndexToLocation(InVoxel->GetIndex()).Z + AVoxelModule::GetWorldData()->BlockSize);
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
