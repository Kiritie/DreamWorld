// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
		
	GENERATED_MODULE(ADWVoxelModule)

public:	
	// Sets default values for this actor's properties
	ADWVoxelModule();

	~ADWVoxelModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation(EPhase InPhase) override;

public:
	virtual FVoxelWorldSaveData* NewWorldData(FSaveData* InBasicData) const override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(EPhase InPhase) override;

	//////////////////////////////////////////////////////////////////////////
	// Components
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BoundsMesh;

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
