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
		
	MODULE_INSTANCE_DECLARE(ADWVoxelModule)

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
	virtual void GenerateVoxels() override;

	virtual void GenerateWorld() override;

public:
	virtual void BuildChunkMap(FIndex InIndex, int32 InStage) override;

	virtual void BuildChunkMesh(FIndex InIndex) override;

	virtual void GenerateChunk(FIndex InIndex) override;

	virtual void DestroyChunk(FIndex InIndex) override;

public:
	virtual AVoxelChunk* SpawnChunk(FIndex InIndex, bool bAddToQueue = true) override;

	virtual ECollisionChannel GetChunkTraceType() const override;

	virtual ECollisionChannel GetVoxelTraceType() const override;
};
