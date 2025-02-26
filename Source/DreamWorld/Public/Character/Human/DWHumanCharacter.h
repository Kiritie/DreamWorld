// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/DWCharacter.h"
#include "DWHumanCharacter.generated.h"

class AVoxelEntity;
class UStaticMeshComponent;

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
	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;

	virtual void OnPreChangeItem(const FAbilityItem& InOldItem) override;
	
	virtual void OnChangeItem(const FAbilityItem& InNewItem) override;
	
	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;

	virtual void OnDeactiveItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

public:
	virtual void SetHitAble(bool bValue) override;

	virtual void ClearHitTargets() override;

	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GenerateToolMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	AVoxelEntity* GenerateVoxelEntity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	AVoxelEntity* AuxiliaryVoxelEntity;
};
