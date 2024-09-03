// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Spawner/AbilityPlayerSpawner.h"
#include "Common/DWCommonTypes.h"
#include "DWPlayerSpawner.generated.h"

UCLASS()
class DREAMWORLD_API ADWPlayerSpawner : public AAbilityPlayerSpawner
{
	GENERATED_BODY()

public:
	ADWPlayerSpawner();

protected:
	virtual AActor* SpawnImpl_Implementation(const FAbilityItem& InAbilityItem) override;

	virtual void DestroyImpl_Implementation(AActor* InAbilityActor) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	EDWInventoryInitType InventoryInitType;
};
