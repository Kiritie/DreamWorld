// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Hitter/DWAbilityHitterComponent.h"

#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Audio/AudioModuleStatics.h"
#include "Kismet/GameplayStatics.h"

UDWAbilityHitterComponent::UDWAbilityHitterComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttackHitSound = nullptr;
	AttackMissSound = nullptr;
	AttackHitEffect = nullptr;
	AttackMissEffect = nullptr;
}

bool UDWAbilityHitterComponent::CanHitTarget(AActor* InTarget) const
{
	return Super::CanHitTarget(InTarget);
}

void UDWAbilityHitterComponent::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	Super::OnHitTarget(InTarget, InHitResult);

	const FVector HitLocation = GetComponentLocation();
	UAudioModuleStatics::PlaySoundAtLocation(InTarget->Implements<UAbilityVitalityInterface>() ? AttackHitSound : AttackMissSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, InTarget->Implements<UAbilityVitalityInterface>() ? AttackHitEffect : AttackMissEffect, HitLocation);
}

void UDWAbilityHitterComponent::ClearHitTargets()
{
	Super::ClearHitTargets();
}

void UDWAbilityHitterComponent::SetHitAble(bool bValue)
{
	Super::SetHitAble(bValue);
}
