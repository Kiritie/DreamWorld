// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/PickUpVoxel.h"
#include "Voxel/Voxel.h"
#include "Components/BoxComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "World/WorldManager.h"

// Sets default values
APickUpVoxel::APickUpVoxel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UVoxelMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickUpVoxel::BeginPlay()
{
	Super::BeginPlay();

}

void APickUpVoxel::Initialize(FItem InItem, bool bPreview /*= false*/)
{
	Super::Initialize(InItem, bPreview);
	BoxComponent->SetBoxExtent(GetVoxelData().Range * AWorldManager::GetData().BlockSize * (1 / GetVoxelData().Range.Z) * 0.2f);
	Cast<UVoxelMeshComponent>(MeshComponent)->Initialize(!bPreview ? EVoxelMeshType::PickUp : EVoxelMeshType::PreviewItem);
	Cast<UVoxelMeshComponent>(MeshComponent)->BuildVoxel(UVoxel::NewVoxel(this, GetVoxelData().VoxelType));
	Cast<UVoxelMeshComponent>(MeshComponent)->CreateMesh(0, false);
}

FVoxelData APickUpVoxel::GetVoxelData()
{
	return UDWHelper::LoadVoxelData(Item.ID);
}

void APickUpVoxel::OnPickUp(ADWCharacter* InPicker)
{
	Super::OnPickUp(InPicker);
}

// Called every frame
void APickUpVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
