// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWVitalityVoxel.h"
#include "DWVitalityVoxelPlant.generated.h"

class UVoxelMeshComponent;

/**
 * 有生命的植物
 */
UCLASS()
class DREAMWORLD_API ADWVitalityVoxelPlant : public ADWVitalityVoxel
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWVitalityVoxelPlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
