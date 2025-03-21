// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/Voxel/DWVitalityVoxel.h"

#include "Asset/AssetModuleStatics.h"
#include "Components/BoxComponent.h"
#include "Common/DWCommonTypes.h"
#include "Vitality/Voxel/DWVitalityVoxelData.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Data/VoxelData.h"

// Sets default values
ADWVitalityVoxel::ADWVitalityVoxel(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	VoxelMesh = CreateDefaultSubobject<UVoxelMeshComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(RootComponent);
	VoxelMesh->Initialize(EVoxelScope::Vitality);
}

void ADWVitalityVoxel::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	VoxelID = FPrimaryAssetId();
}

void ADWVitalityVoxel::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	if(PHASEC(InPhase, EPhase::Final))
	{
		VoxelID = GetVitalityData<UDWVitalityVoxelData>().VoxelID;
		if(VoxelID.IsValid())
		{
			VoxelMesh->CreateVoxel(VoxelID);
			const FVector Range = GetVoxelData().GetRange();
			BoxComponent->SetBoxExtent(Range * UVoxelModule::Get().GetWorldData().BlockSize * 0.5f);
		}
	}

}

FSaveData* ADWVitalityVoxel::ToData()
{
	return Super::ToData();
}

UVoxelData& ADWVitalityVoxel::GetVoxelData() const
{
	return UAssetModuleStatics::LoadPrimaryAssetRef<UVoxelData>(VoxelID);
}

UMeshComponent* ADWVitalityVoxel::GetMeshComponent() const
{
	return VoxelMesh;
}
