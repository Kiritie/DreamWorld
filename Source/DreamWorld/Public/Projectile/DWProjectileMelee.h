// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Projectile/AbilityProjectileMeleeBase.h"
#include "DWProjectileMelee.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWProjectileMelee : public AAbilityProjectileMeleeBase
{
	GENERATED_BODY()
	
public:	
	ADWProjectileMelee();

public:
	virtual void Initialize_Implementation(AActor* InOwnerActor, const FGameplayAbilitySpecHandle& InAbilityHandle) override;
};
