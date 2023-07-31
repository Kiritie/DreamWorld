// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

public:
	template<class T>
	T& GetVoxelData() const
	{
		return static_cast<T&>(GetVoxelData());
	}
	UVoxelData& GetVoxelData() const;
};
