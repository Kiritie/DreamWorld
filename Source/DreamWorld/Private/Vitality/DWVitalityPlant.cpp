// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/VitalityPlant.h"
#include "Components/BoxComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxel.h"

// Sets default values
ADWVitalityPlant::ADWVitalityPlant()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADWVitalityPlant::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADWVitalityPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
