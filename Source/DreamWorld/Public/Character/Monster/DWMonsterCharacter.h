// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/DWCharacter.h"
#include "DWMonsterCharacter.generated.h"

class UDWCharacterAttackPoint;

/**
 * �����ɫ
 */
UCLASS()
class DREAMWORLD_API ADWMonsterCharacter : public ADWCharacter
{
	GENERATED_BODY()
	
public:
	ADWMonsterCharacter();

public:
	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;
	
	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity) override;

public:
	virtual void SetAttackHitAble(bool bValue) override;

	virtual void ClearAttackHitTargets() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UDWCharacterAttackPoint* AttackPoint;

public:
	UFUNCTION(BlueprintPure)
	TArray<UDWCharacterAttackPoint*> GetAttackPoints();
};
