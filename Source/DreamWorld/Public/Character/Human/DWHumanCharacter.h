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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* HammerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AVoxelEntity* GenerateVoxelEntity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AVoxelEntity* AuxiliaryVoxelEntity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

	virtual void RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem) override;

public:
	virtual void SetAttackHitAble(bool bValue) override;

	virtual void ClearAttackHitTargets() override;

	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;
};
