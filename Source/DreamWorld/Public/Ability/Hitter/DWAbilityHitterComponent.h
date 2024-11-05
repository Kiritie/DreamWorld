// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Hitter/AbilityHitterComponentBase.h"

#include "DWAbilityHitterComponent.generated.h"

/**
 * 攻击点组件
 */
UCLASS()
class DREAMWORLD_API UDWAbilityHitterComponent : public UAbilityHitterComponentBase
{
	GENERATED_BODY()
	
public:
	UDWAbilityHitterComponent(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanHitTarget(AActor* InTarget) const override;

	virtual void OnHitTarget(AActor* InTarget, const FHitResult& InHitResult) override;
	
	virtual void ClearHitTargets() override;

	virtual void SetHitAble(bool bValue) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackHitEffect;
};
