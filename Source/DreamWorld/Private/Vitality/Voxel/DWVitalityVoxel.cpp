// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/Voxel/DWVitalityVoxel.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Components/BoxComponent.h"
#include "Global/DWGlobalTypes.h"
#include "Vitality/Voxel/DWVitalityVoxelData.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Datas/VoxelData.h"

// Sets default values
ADWVitalityVoxel::ADWVitalityVoxel()
{
	VoxelMesh = CreateDefaultSubobject<UVoxelMeshComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(RootComponent);
	VoxelMesh->Initialize(EVoxelMeshNature::Vitality);
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
			BoxComponent->SetBoxExtent(Range * AVoxelModule::Get()->GetWorldData().BlockSize * 0.5f);
		}
	}

}

FSaveData* ADWVitalityVoxel::ToData(bool bRefresh)
{
	return Super::ToData(bRefresh);
}

UVoxelData& ADWVitalityVoxel::GetVoxelData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAssetRef<UVoxelData>(VoxelID);
}
