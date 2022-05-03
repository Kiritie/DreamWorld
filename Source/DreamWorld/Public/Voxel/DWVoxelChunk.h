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
class APickUp;
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

	virtual void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSpawn_Implementation() override;
		
	virtual void OnDespawn_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// Stats
protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<ADWCharacter*> Characters;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<ADWVitality*> Vitalitys;

	//////////////////////////////////////////////////////////////////////////
	// Chunk
public:
	virtual void Initialize(FIndex InIndex, int32 InBatch) override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData(bool bSaved = true) override;

	virtual void Generate(bool bPreview = false) override;

	virtual void BuildMap() override;

	virtual void GenerateMap() override;

	virtual void SpawnActors() override;

	virtual void DestroyActors() override;

protected:
	virtual void OnGenerated(bool bPreview = false) override;

	//////////////////////////////////////////////////////////////////////////
	// SceneContainer
public:
	virtual void AddSceneActor(AActor* InActor) override;

	virtual void RemoveSceneActor(AActor* InActor) override;

	virtual void DestroySceneActor(AActor* InActor) override;

	//////////////////////////////////////////////////////////////////////////
	// Character
public:
	ADWCharacter* SpawnCharacter(FDWCharacterSaveData InSaveData);

	void AttachCharacter(ADWCharacter* InCharacter);

	void DetachCharacter(ADWCharacter* InCharacter);

	void DestroyCharacter(ADWCharacter* InCharacter);

	//////////////////////////////////////////////////////////////////////////
	// Vitality
public:
	ADWVitality* SpawnVitality(FDWVitalitySaveData InSaveData);

	void AttachVitality(ADWVitality* InVitality);

	void DetachVitality(ADWVitality* InVitality);

	void DestroyVitality(ADWVitality* InVitality);
};

