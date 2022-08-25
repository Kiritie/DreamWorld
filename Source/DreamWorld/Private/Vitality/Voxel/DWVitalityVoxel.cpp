// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/Voxel/DWVitalityVoxel.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Components/BoxComponent.h"
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

// Called when the game starts or when spawned
void ADWVitalityVoxel::BeginPlay()
{
	Super::BeginPlay();
}

void ADWVitalityVoxel::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();

	VoxelID = FPrimaryAssetId();
}

void ADWVitalityVoxel::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);

	auto& SaveData = InSaveData->CastRef<FDWVitalitySaveData>();

	VoxelID = GetVitalityData<UDWVitalityVoxelData>().VoxelID;
	if(VoxelID.IsValid())
	{
		VoxelMesh->CreateVoxel(VoxelID);
		const FVector range = GetVoxelData().GetRange();
        BoxComponent->SetBoxExtent(range * UVoxelModuleBPLibrary::GetWorldData().BlockSize * 0.5f);
	}
}

FSaveData* ADWVitalityVoxel::ToData()
{
	return Super::ToData();
}

// Called every frame
void ADWVitalityVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UVoxelData& ADWVitalityVoxel::GetVoxelData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAssetRef<UVoxelData>(VoxelID);
}
