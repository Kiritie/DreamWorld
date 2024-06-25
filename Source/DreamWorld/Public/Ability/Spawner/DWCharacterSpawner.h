// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Spawner/AbilityCharacterSpawner.h"
#include "DWCharacterSpawner.generated.h"

UCLASS()
class DREAMWORLD_API ADWCharacterSpawner : public AAbilityCharacterSpawner
{
	GENERATED_BODY()

public:
	ADWCharacterSpawner();

protected:
	virtual AActor* SpawnImpl_Implementation(const FAbilityItem& InAbilityItem) override;

	virtual void DestroyImpl_Implementation(AActor* InAbilityActor) override;
};
