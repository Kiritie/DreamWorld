// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "DreamWorld/DreamWorld.h"
#include "DWEquipShield.generated.h"

/**
 * ����
 */
UCLASS()
class DREAMWORLD_API ADWEquipShield : public AAbilityEquipBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWEquipShield();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStats")
	float DefendRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStats")
	float DefendScope;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
