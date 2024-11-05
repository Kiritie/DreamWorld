// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterPart.h"

#include "Character/DWCharacter.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Datas/VoxelData.h"

UDWCharacterPart::UDWCharacterPart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CharacterPartType = EDWCharacterPartType::None;
}

void UDWCharacterPart::BeginPlay()
{
	Super::BeginPlay();
}

void UDWCharacterPart::OnHitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	Super::OnHitVoxel(InVoxel, InHitResult);
}

void UDWCharacterPart::OnEnterVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	Super::OnEnterVoxel(InVoxel, InHitResult);
}

void UDWCharacterPart::OnStayVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	Super::OnStayVoxel(InVoxel, InHitResult);

	const auto OwnerCharacter = GetOwnerCharacter<ADWCharacter>();

	const UVoxelData& VoxelData = InVoxel.GetData();
	switch (VoxelData.VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case EDWCharacterPartType::Chest:
				{
					if(OwnerCharacter->IsFreeToAnim() && !OwnerCharacter->IsSwimming())
					{
						OwnerCharacter->Float(InVoxel.GetOwner()->IndexToLocation(InVoxel.GetIndex()).Z + UVoxelModule::Get().GetWorldData().BlockSize);
					}
					break;
				}
				case EDWCharacterPartType::Neck:
				{
					if(OwnerCharacter->IsFreeToAnim())
					{
						OwnerCharacter->Swim();
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

void UDWCharacterPart::OnExitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult)
{
	Super::OnExitVoxel(InVoxel, InHitResult);

	const auto OwnerCharacter = GetOwnerCharacter<ADWCharacter>();

	const UVoxelData& VoxelData = InVoxel.GetData();
	switch (VoxelData.VoxelType)
	{
		case EVoxelType::Water:
		{
			switch (CharacterPartType)
			{
				case EDWCharacterPartType::Chest:
				{
					if(!InHitResult.IsValid() || InHitResult.VoxelItem.GetVoxelType() != EVoxelType::Water)
					{
						OwnerCharacter->UnFloat();
					}
					break;
				}
				case EDWCharacterPartType::Neck:
				{
					if(!InHitResult.IsValid() || InHitResult.VoxelItem.GetVoxelType() != EVoxelType::Water)
					{
						OwnerCharacter->Float(InVoxel.GetOwner()->IndexToLocation(InVoxel.GetIndex()).Z + UVoxelModule::Get().GetWorldData().BlockSize);
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
