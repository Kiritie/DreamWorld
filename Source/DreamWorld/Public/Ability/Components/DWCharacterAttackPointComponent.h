// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Components/CharacterAttackPointComponent.h"

#include "DWCharacterAttackPointComponent.generated.h"

/**
 * 角色攻击点组件
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAttackPointComponent : public UCharacterAttackPointComponent
{
	GENERATED_BODY()
	
public:
	UDWCharacterAttackPointComponent(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanHitTarget_Implementation(AActor* InTarget) override;

	virtual void OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult) override;
	
	virtual void ClearHitTargets_Implementation();

	virtual void SetHitAble_Implementation(bool bValue);
};
