// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Spawner/AbilityVitalitySpawner.h"
#include "DWVitalitySpawner.generated.h"

UCLASS()
class DREAMWORLD_API ADWVitalitySpawner : public AAbilityVitalitySpawner
{
	GENERATED_BODY()

public:
	ADWVitalitySpawner();

protected:
	virtual AActor* SpawnImpl_Implementation(const FAbilityItem& InAbilityItem) override;

	virtual void DestroyImpl_Implementation(AActor* InAbilityActor) override;
};
