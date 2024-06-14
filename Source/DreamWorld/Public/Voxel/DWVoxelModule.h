// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Voxel/VoxelModule.h"
#include "DWVoxelModule.generated.h"

/**
 * 体素模块
 */
UCLASS()
class DREAMWORLD_API UDWVoxelModule : public UVoxelModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWVoxelModule)

public:	
	// Sets default values for this actor's properties
	UDWVoxelModule();

	~UDWVoxelModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds, bool bInEditor) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

	virtual void OnTermination(EPhase InPhase) override;

public:
	virtual FVoxelWorldSaveData* NewWorldData(FSaveData* InBasicData) const override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(EPhase InPhase) override;

	//////////////////////////////////////////////////////////////////////////
	// Bounds
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bounds")
	AActor* BoundsMesh;

	//////////////////////////////////////////////////////////////////////////
	// World
protected:
	virtual void OnWorldStateChanged() override;

protected:
	virtual void GenerateWorld() override;

public:
	virtual void BuildChunkMap(FIndex InIndex, int32 InStage) override;

	virtual void BuildChunkMesh(FIndex InIndex) override;

	virtual void GenerateChunk(FIndex InIndex) override;

	virtual void DestroyChunk(FIndex InIndex) override;

public:
	virtual AVoxelChunk* SpawnChunk(FIndex InIndex, bool bAddToQueue = true) override;

	virtual ECollisionChannel GetChunkTraceChannel() const override;

	virtual ECollisionChannel GetVoxelTraceChannel() const override;
};
