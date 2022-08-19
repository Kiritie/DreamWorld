// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
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
	UChildActorComponent* VoxelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* HammerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;

	virtual void SetGenerateVoxelItem(const FVoxelItem& InGenerateVoxelItem) override;

	virtual void SetAttackDamageAble(bool bInDamaging) override;
};
