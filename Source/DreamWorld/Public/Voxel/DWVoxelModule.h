// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "Voxel/VoxelModule.h"
#include "DWVoxelModule.generated.h"

/**
 * 体素模块
 */
UCLASS()
class DREAMWORLD_API ADWVoxelModule : public AVoxelModule
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWVoxelModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// Components
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BoundsMesh;

	//////////////////////////////////////////////////////////////////////////
	// World
protected:
	virtual void OnWorldStateChanged() override;
	
public:
	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData() override;

protected:
	virtual void GeneratePreviews() override;

	virtual void GenerateTerrain() override;

	virtual void GenerateChunks(FIndex InIndex) override;

public:
	virtual void BuildChunkMap(AVoxelChunk* InChunk) override;

	virtual void GenerateChunkMap(AVoxelChunk* InChunk) override;

	virtual void GenerateChunk(AVoxelChunk* InChunk) override;

	virtual void DestroyChunk(AVoxelChunk* InChunk) override;

public:
	virtual AVoxelChunk* SpawnChunk(FIndex InIndex, bool bAddToQueue = true) override;

public:
	virtual bool ChunkTraceSingle(AVoxelChunk* InChunk, float InRadius, float InHalfHeight, FHitResult& OutHitResult) override;

	virtual bool ChunkTraceSingle(FVector RayStart, FVector RayEnd, float InRadius, float InHalfHeight, FHitResult& OutHitResult) override;

	virtual bool VoxelTraceSingle(const FVoxelItem& InVoxelItem, FVector InPoint, FHitResult& OutHitResult) override;
};
