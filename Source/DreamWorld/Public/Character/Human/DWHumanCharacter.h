// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/DWCharacter.h"
#include "DWHumanCharacter.generated.h"

class AVoxelEntity;
class UBoxComponent;
class UStaticMeshComponent;
class ADWEquipWeapon;
class ADWEquipShield;
class ADWEquipArmor;
class UAbilityInventoryEquipSlotBase;

/**
 * 类人角色
 */
UCLASS()
class DREAMWORLD_API ADWHumanCharacter : public ADWCharacter
{
	GENERATED_BODY()

public:
	ADWHumanCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;

	virtual void OnDeactiveItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnSelectItem(ESlotSplitType InSplitType, const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GenerateHammerMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AVoxelEntity* GenerateVoxelEntity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AVoxelEntity* AuxiliaryVoxelEntity;

public:
	virtual void SetAttackHitAble(bool bValue) override;

	virtual void ClearAttackHitTargets() override;

	virtual void SetControlMode_Implementation(EDWCharacterControlMode InControlMode) override;
};
