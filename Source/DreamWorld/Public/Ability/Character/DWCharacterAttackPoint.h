// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterAttackPointBase.h"

#include "DWCharacterAttackPoint.generated.h"

/**
 * 角色攻击点组件
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAttackPoint : public UAbilityCharacterAttackPointBase
{
	GENERATED_BODY()
	
public:
	UDWCharacterAttackPoint(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanHitTarget_Implementation(AActor* InTarget) override;

	virtual void OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult) override;
	
	virtual void ClearHitTargets_Implementation() override;

	virtual void SetHitAble_Implementation(bool bValue) override;
};
