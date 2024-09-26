// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Projectile/AbilityProjectileRemoteBase.h"
#include "DWProjectileRemote.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWProjectileRemote : public AAbilityProjectileRemoteBase
{
	GENERATED_BODY()
	
public:	
	ADWProjectileRemote();

public:
	virtual void Initialize_Implementation(AActor* InOwnerActor, const FGameplayAbilitySpecHandle& InAbilityHandle) override;

public:
	virtual void OnHitTarget(AActor* InTarget, const FHitResult& InHitResult) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackHitEffect;
};
