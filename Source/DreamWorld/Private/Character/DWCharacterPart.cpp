// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterPart.h"

#include "Character/DWCharacter.h"
#include "Voxel/Voxel.h"
#include "World/Chunk.h"

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
		const UVoxelAssetBase VoxelData = VoxelItem.GetVoxelData();
		const FVoxelHitResult VoxelHitResult = FVoxelHitResult(VoxelItem, GetComponentLocation(), GetOwnerCharacter()->GetMoveDirection(false));
		if(VoxelItem.IsValid())
		{
			if(!LastOverlapVoxel || LastOverlapVoxel->GetIndex() != VoxelItem.Index)
			{
				if(LastOverlapVoxel)
				{
					LastOverlapVoxel->OnTargetExit(this, VoxelHitResult);
					UVoxel::DespawnVoxel(LastOverlapVoxel);
					LastOverlapVoxel = nullptr;
				}
				if(UVoxel* Voxel = VoxelItem.GetVoxel())
				{
					LastOverlapVoxel = Voxel;
					Voxel->OnTargetEnter(this, VoxelHitResult);
				}
			}
		}
		else if(LastOverlapVoxel)
		{
			LastOverlapVoxel->OnTargetExit(this, VoxelHitResult);
			UVoxel::DespawnVoxel(LastOverlapVoxel);
			LastOverlapVoxel = nullptr;
		}
	}
}

ADWCharacter* UDWCharacterPart::GetOwnerCharacter() const
{
	return Cast<ADWCharacter>(GetOwner());
}
