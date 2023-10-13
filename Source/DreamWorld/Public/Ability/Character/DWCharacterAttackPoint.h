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
	virtual bool CanHitTarget(AActor* InTarget) const override;

	virtual void OnHitTarget(AActor* InTarget, const FHitResult& InHitResult) override;
	
	virtual void ClearHitTargets() override;

	virtual void SetHitAble(bool bValue) override;
};
