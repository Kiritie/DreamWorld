// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/DWCharacter.h"
#include "DWMonsterCharacter.generated.h"

class UDWAbilityHitterComponent;

/**
 * �����ɫ
 */
UCLASS()
class DREAMWORLD_API ADWMonsterCharacter : public ADWCharacter
{
	GENERATED_BODY()
	
public:
	ADWMonsterCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;
	
	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;

public:
	virtual void SetHitAble(bool bValue) override;

	virtual void ClearHitTargets() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UDWAbilityHitterComponent* AttackPoint;

public:
	UFUNCTION(BlueprintPure)
	TArray<UDWAbilityHitterComponent*> GetAttackPoints() const;
};
