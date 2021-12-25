// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SpawnPool/SpawnPoolInterface.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

class UVoxel;
class APickUp;
class AVitalityObject;
class UVoxelMeshComponent;

/**
 * 体素块
 */
UCLASS()
class DREAMWORLD_API AChunk : public AActor, public ISpawnPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSpawn_Implementation() override;
		
	virtual void OnDespawn_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// Components
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* SolidMesh;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* SemiMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* TransMesh;

public:
	UVoxelMeshComponent* GetSolidMesh();

	UVoxelMeshComponent* GetSemiMesh();

	UVoxelMeshComponent* GetTransMesh();
	
public:
	void DestroySolidMesh();

	void DestroySemiMesh();

	void DestroyTransMesh();

	//////////////////////////////////////////////////////////////////////////
	// Stats
protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<AChunk*> Neighbors;
	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TMap<FIndex, FVoxelItem> VoxelMap;
		
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<ADWCharacter*> Characters;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<AVitalityObject*> VitalityObjects;
	
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TArray<APickUp*> PickUps;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	FIndex Index;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Batch;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	bool bGenerated;

public:
	FIndex GetIndex() const { return Index; }

	int32 GetBatch() const { return Batch; }
	
	bool IsGenerated() const { return bGenerated; }

	TArray<AChunk*> GetNeighbors() const { return Neighbors; }

	//////////////////////////////////////////////////////////////////////////
	// Chunk
public:
	void Initialize(FIndex InIndex, int32 InBatch);

	void Generate();

	void GenerateMap();

	void BuildMap();

	void LoadMap(FChunkSaveData InChunkData);

	void OnGenerated(bool bPreview = false);

protected:
	void LoadActors(FChunkSaveData InChunkData);
		
	void GenerateNeighbors(FIndex InIndex);

	void GenerateNeighbors(int32 InX, int32 InY, int32 InZ);

	void UpdateNeighbors();

	void BreakNeighbors();

public:
	bool IsOnTheChunk(FIndex InIndex) const;

	bool IsOnTheChunk(FVector InLocation) const;

	FIndex LocationToIndex(FVector InLocation, bool bWorldSpace = true) const;

	FVector IndexToLocation(FIndex InIndex, bool bWorldSpace = true) const;

	FIndex LocalIndexToWorld(FIndex InIndex) const;

	FIndex WorldIndexToLocal(FIndex InIndex) const;

	//////////////////////////////////////////////////////////////////////////
	// Voxel
public:
	UVoxel* GetVoxel(FIndex InIndex);

	UVoxel* GetVoxel(int32 InX, int32 InY, int32 InZ);

	FVoxelItem& GetVoxelItem(FIndex InIndex);

	FVoxelItem& GetVoxelItem(int32 InX, int32 InY, int32 InZ);

public:
	bool CheckVoxel(FIndex InIndex, FVector InRange = FVector::OneVector, bool bIgnoreTransparent = true);

	bool CheckVoxel(int32 InX, int32 InY, int32 InZ, FVector InRange = FVector::OneVector, bool bIgnoreTransparent = true);

	bool CheckAdjacent(const FVoxelItem& InVoxelItem, EDirection InDirection);

	bool CheckNeighbors(const FVoxelItem& InVoxelItem, EVoxelType InVoxelType, bool bIgnoreBottom = false, int32 InDistance = 1);

	bool SetVoxelSample(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerateMesh = false);

	bool SetVoxelSample(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerateMesh = false);

	bool SetVoxelComplex(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerateMesh = false);

	bool SetVoxelComplex(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerateMesh = false);

	bool GenerateVoxel(FIndex InIndex, const FVoxelItem& InVoxelItem);

	bool DestroyVoxel(FIndex InIndex);

	bool DestroyVoxel(const FVoxelItem& InVoxelItem);

	bool ReplaceVoxel(const FVoxelItem& InOldVoxelItem, const FVoxelItem& InNewVoxelItem);

	//////////////////////////////////////////////////////////////////////////
	// Auxiliary
public:
	AVoxelAuxiliary* SpawnAuxiliary(FVoxelItem& InVoxelItem);

	void DestroyAuxiliary(AVoxelAuxiliary* InAuxiliary);

	//////////////////////////////////////////////////////////////////////////
	// PickUp
public:
	APickUp* SpawnPickUp(FItem InItem, FVector InLocation);

	APickUp* SpawnPickUp(FPickUpSaveData InPickUpData);

	void AttachPickUp(APickUp* InPickUp);

	void DetachPickUp(APickUp* InPickUp);

	void DestroyPickUp(APickUp* InPickUp);

	//////////////////////////////////////////////////////////////////////////
	// Character
public:
	ADWCharacter* SpawnCharacter(FCharacterSaveData InSaveData);

	void AttachCharacter(ADWCharacter* InCharacter);

	void DetachCharacter(ADWCharacter* InCharacter);

	void DestroyCharacter(ADWCharacter* InCharacter);

	//////////////////////////////////////////////////////////////////////////
	// VitalityObject
public:
	AVitalityObject* SpawnVitalityObject(FVitalityObjectSaveData InSaveData);

	void AttachVitalityObject(AVitalityObject* InVitalityObject);

	void DetachVitalityObject(AVitalityObject* InVitalityObject);

	void DestroyVitalityObject(AVitalityObject* InVitalityObject);
};

