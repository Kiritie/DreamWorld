// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "GameFramework/Actor.h"
#include "Voxel/VoxelModuleTypes.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "DWVoxelChunk.generated.h"

class ADWVitality;
class UVoxel;
class AAbilityPickUpBase;
class UVoxelMeshComponent;

/**
 * 体素块
 */
UCLASS()
class DREAMWORLD_API ADWVoxelChunk : public AVoxelChunk
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWVoxelChunk();

protected:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;
		
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//////////////////////////////////////////////////////////////////////////
	// Chunk
public:
	virtual void Initialize(FIndex InIndex, int32 InBatch) override;

	virtual void Generate(EPhase InPhase) override;

	virtual void BuildMap(int32 InStage) override;

	virtual void BuildMesh() override;

	virtual void LoadSceneActors(FSaveData* InSaveData) override;

	virtual void SpawnSceneActors() override;
};

