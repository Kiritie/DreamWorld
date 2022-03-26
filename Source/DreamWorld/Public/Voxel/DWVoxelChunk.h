// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWTypes.h"
#include "SpawnPool/SpawnPoolInterface.h"
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
class WHFRAMEWORK_API ADWVoxelChunk : public AVoxelChunk, public ISpawnPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWVoxelChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
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

	virtual void LoadData(FSaveData* InChunkData) override;

	virtual FSaveData* ToData(bool bSaved = true) override;

	virtual void Generate(bool bPreview = false) override;

	virtual void BuildMap() override;

	virtual void GenerateMap() override;

	virtual void SpawnActors() override;

	virtual void DestroyActors() override;

protected:
	virtual void OnGenerated(bool bPreview = false) override;

	//////////////////////////////////////////////////////////////////////////
	// Character
public:
	ACharacterBase* SpawnCharacter(FCharacterSaveData InSaveData);

	void AttachCharacter(ACharacterBase* InCharacter);

	void DetachCharacter(ACharacterBase* InCharacter);

	void DestroyCharacter(ACharacterBase* InCharacter);

	//////////////////////////////////////////////////////////////////////////
	// Vitality
public:
	ADWVitality* SpawnVitality(FVitalitySaveData InSaveData);

	void AttachVitality(ADWVitality* InVitality);

	void DetachVitality(ADWVitality* InVitality);

	void DestroyVitality(ADWVitality* InVitality);
};

