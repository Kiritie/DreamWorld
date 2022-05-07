// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitalityVoxel.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Components/BoxComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Datas/VoxelData.h"

// Sets default values
ADWVitalityVoxel::ADWVitalityVoxel()
{
	VoxelMesh = CreateDefaultSubobject<UVoxelMeshComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(RootComponent, FName("VoxelMesh"));
	VoxelMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	VoxelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VoxelID = FPrimaryAssetId();
}

// Called when the game starts or when spawned
void ADWVitalityVoxel::BeginPlay()
{
	Super::BeginPlay();

	FVector range = GetVoxelData().GetFinalRange();
	BoxComponent->SetBoxExtent(range * AVoxelModule::GetWorldData()->BlockSize);
	//AddActorLocalOffset(FVector::UpVector * range.Z * VoxelMesh->BlockScale * 0.5f);

	if (GetVoxelData().IsValid())
	{
		VoxelMesh->Initialize(EVoxelMeshType::Vitality);
		VoxelMesh->BuildVoxel(FVoxelItem(VoxelID));
		VoxelMesh->CreateMesh(0, false);
	}
	else
	{
		VoxelMesh->ClearMesh();
	}
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
