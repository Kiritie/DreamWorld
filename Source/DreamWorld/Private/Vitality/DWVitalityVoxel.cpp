// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/VitalityVoxel.h"
#include "Components/BoxComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxel.h"
#include "World/VoxelModule.h"

// Sets default values
ADWVitalityVoxel::ADWVitalityVoxel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VoxelMesh = CreateDefaultSubobject<UVoxelMeshComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(RootComponent, FName("VoxelMesh"));
	VoxelMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	VoxelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VoxelID = NAME_None;
}

// Called when the game starts or when spawned
void ADWVitalityVoxel::BeginPlay()
{
	Super::BeginPlay();

	FVector range = GetVoxelData().GetFinalRange();
	BoxComponent->SetBoxExtent(range * AVoxelModule::GetWorldData().BlockSize);
	//AddActorLocalOffset(FVector::UpVector * range.Z * VoxelMesh->BlockScale * 0.5f);

	if (GetVoxelData().IsValid())
	{
		VoxelMesh->Initialize(EVoxelMeshType::VitalityVoxel);
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

UVoxelAssetBase ADWVitalityVoxel::GetVoxelData() const
{
	return UDWHelper::LoadVoxelData(VoxelID);
}
