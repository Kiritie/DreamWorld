// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWTypes.h"
#include "ReferencePool/ReferencePoolInterface.h"
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;
		
	virtual void OnDespawn_Implementation() override;

	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//////////////////////////////////////////////////////////////////////////
	// Stats
protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<ADWCharacter*> Characters;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<ADWVitality*> Vitalitys;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	bool bRaceGenerated;

	//////////////////////////////////////////////////////////////////////////
	// Chunk
public:
	virtual void Initialize(AVoxelModule* InModule, FIndex InIndex, int32 InBatch) override;

	virtual void Generate(bool bBuildMesh, bool bForceMode) override;

	virtual void BuildMap(int32 InStage) override;

	virtual void BuildMesh() override;

	virtual void SpawnActors() override;

	virtual void LoadActors(FSaveData* InSaveData) override;

	virtual void GenerateActors() override;

	virtual void DestroyActors() override;

	//////////////////////////////////////////////////////////////////////////
	// SceneContainer
public:
	virtual void AddSceneActor(AActor* InActor) override;

	virtual void RemoveSceneActor(AActor* InActor) override;
};

