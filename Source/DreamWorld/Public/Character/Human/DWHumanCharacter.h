// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Character/DWCharacter.h"
#include "DWHumanCharacter.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ADWEquipWeapon;
class ADWEquipShield;
class ADWEquipArmor;
class UInventoryEquipSlot;

/**
 * 类人角色
 */
UCLASS()
class DREAMWORLD_API ADWHumanCharacter : public ADWCharacter
{
	GENERATED_BODY()

public:
	ADWHumanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetDamageAble(bool bInDamaging) override;
};
