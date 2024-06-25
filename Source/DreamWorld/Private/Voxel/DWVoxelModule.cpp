// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Components/StaticMeshComponent.h"
#include "Common/CommonStatics.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"

IMPLEMENTATION_MODULE(UDWVoxelModule)

// Sets default values
UDWVoxelModule::UDWVoxelModule()
{
	ChunkSpawnClass = ADWVoxelChunk::StaticClass();

	BoundsMesh = nullptr;
}

UDWVoxelModule::~UDWVoxelModule()
{
	TERMINATION_MODULE(UDWVoxelModule)
}

#if WITH_EDITOR
void UDWVoxelModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWVoxelModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWVoxelModule)
}
#endif

void UDWVoxelModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWVoxelModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWVoxelModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWVoxelModule::OnPause()
{
	Super::OnPause();
}

void UDWVoxelModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWVoxelModule::OnTermination(EPhase InPhase)
{
	Super::OnTermination(InPhase);
}

FVoxelWorldSaveData* UDWVoxelModule::NewWorldData(FSaveData* InBasicData) const
{
	static FDWWorldSaveData SaveData;
	SaveData = !InBasicData ? FDWWorldSaveData(WorldBasicData) : InBasicData->CastRef<FDWWorldSaveData>();
	return &SaveData;
}

void UDWVoxelModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWVoxelModule::ToData()
{
	return Super::ToData();
}

void UDWVoxelModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

void UDWVoxelModule::OnWorldStateChanged()
{
	Super::OnWorldStateChanged();
}

void UDWVoxelModule::GenerateWorld()
{
	Super::GenerateWorld();

	if(BoundsMesh && ChunkGenerateIndex != EMPTY_Index)
	{
		BoundsMesh->SetActorLocation(ChunkIndexToLocation(ChunkGenerateIndex));
		BoundsMesh->SetActorScale3D(WorldData->GetWorldRealSize() * 0.01f);
	}
}

void UDWVoxelModule::BuildChunkMap(FIndex InIndex, int32 InStage)
{
	Super::BuildChunkMap(InIndex, InStage);
}

void UDWVoxelModule::BuildChunkMesh(FIndex InIndex)
{
	Super::BuildChunkMesh(InIndex);
}

void UDWVoxelModule::GenerateChunk(FIndex InIndex)
{
	Super::GenerateChunk(InIndex);
}

void UDWVoxelModule::DestroyChunk(FIndex InIndex)
{
	Super::DestroyChunk(InIndex);
}

AVoxelChunk* UDWVoxelModule::SpawnChunk(FIndex InIndex, bool bAddToQueue)
{
	return Super::SpawnChunk(InIndex, bAddToQueue);
}
