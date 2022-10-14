// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Character/DWCharacter.h"
#include "DWMonsterCharacter.generated.h"

class UDWCharacterAttackPointComponent;

/**
 * �����ɫ
 */
UCLASS()
class DREAMWORLD_API ADWMonsterCharacter : public ADWCharacter
{
	GENERATED_BODY()
	
public:
	ADWMonsterCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDWCharacterAttackPointComponent* AttackPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;
	
	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void SetAttackHitAble(bool bValue) override;

	virtual void ClearAttackHitTargets() override;

public:
	UFUNCTION(BlueprintPure)
	TArray<UDWCharacterAttackPointComponent*> GetAttackPoints();
};
