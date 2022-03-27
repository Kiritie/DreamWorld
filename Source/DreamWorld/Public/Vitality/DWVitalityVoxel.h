// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Vitality/AbilityVitalityBase.h"
#include "VitalityVoxel.generated.h"

class UVoxelMeshComponent;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API ADWVitalityVoxel : public AAbilityVitalityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWVitalityVoxel();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName VoxelID;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* VoxelMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	UVoxelAssetBase GetData<UVoxelAssetBase>() const;
};
