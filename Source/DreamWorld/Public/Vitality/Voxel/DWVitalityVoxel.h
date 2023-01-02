// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Vitality/DWVitality.h"

#include "DWVitalityVoxel.generated.h"

class UVoxelMeshComponent;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API ADWVitalityVoxel : public ADWVitality
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWVitalityVoxel();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* VoxelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetId VoxelID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDespawn_Implementation() override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	template<class T>
	T& GetVoxelData() const
	{
		return static_cast<T&>(GetVoxelData());
	}
	UVoxelData& GetVoxelData() const;
};
