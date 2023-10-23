// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Components/StaticMeshComponent.h"
#include "Common/CommonBPLibrary.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWVoxelModule)

// Sets default values
ADWVoxelModule::ADWVoxelModule()
{
	ChunkSpawnClass = ADWVoxelChunk::StaticClass();

	BoundsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoundsMesh"));
	BoundsMesh->SetRelativeScale3D(FVector::ZeroVector);
	BoundsMesh->SetRelativeRotation(FRotator(0, 0, 0));
	BoundsMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

ADWVoxelModule::~ADWVoxelModule()
{
	TERMINATION_MODULE(ADWVoxelModule)
}

#if WITH_EDITOR
void ADWVoxelModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWVoxelModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWVoxelModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWVoxelModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWVoxelModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWVoxelModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWVoxelModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWVoxelModule::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}

FVoxelWorldSaveData* ADWVoxelModule::NewWorldData(FSaveData* InBasicData) const
{
	static FDWWorldSaveData SaveData;
	SaveData = !InBasicData ? FDWWorldSaveData(WorldBasicData) : InBasicData->CastRef<FDWWorldSaveData>();
	return &SaveData;
}

void ADWVoxelModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWVoxelModule::ToData()
{
	return Super::ToData();
}

void ADWVoxelModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

void ADWVoxelModule::OnWorldStateChanged()
{
	Super::OnWorldStateChanged();
}

void ADWVoxelModule::GenerateWorld()
{
	Super::GenerateWorld();

	if(BoundsMesh && ChunkGenerateIndex != Index_Empty)
	{
		BoundsMesh->SetRelativeLocation(ChunkIndexToLocation(ChunkGenerateIndex));
		BoundsMesh->SetRelativeScale3D(WorldData->GetWorldRealSize() * 0.01f);
	}
}

void ADWVoxelModule::BuildChunkMap(FIndex InIndex, int32 InStage)
{
	Super::BuildChunkMap(InIndex, InStage);
}

void ADWVoxelModule::BuildChunkMesh(FIndex InIndex)
{
	Super::BuildChunkMesh(InIndex);
}

void ADWVoxelModule::GenerateChunk(FIndex InIndex)
{
	Super::GenerateChunk(InIndex);
}

void ADWVoxelModule::DestroyChunk(FIndex InIndex)
{
	Super::DestroyChunk(InIndex);
}

AVoxelChunk* ADWVoxelModule::SpawnChunk(FIndex InIndex, bool bAddToQueue)
{
	return Super::SpawnChunk(InIndex, bAddToQueue);
}

ECollisionChannel ADWVoxelModule::GetChunkTraceChannel() const
{
	return (ECollisionChannel)EDWGameTraceChannel::Chunk;
}

ECollisionChannel ADWVoxelModule::GetVoxelTraceChannel() const
{
	return (ECollisionChannel)EDWGameTraceChannel::Voxel;
}
