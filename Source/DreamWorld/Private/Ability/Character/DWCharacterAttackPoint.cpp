// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Character/DWCharacterAttackPoint.h"
#include "Character/Monster/DWMonsterCharacterData.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Monster/DWMonsterCharacter.h"

UDWCharacterAttackPoint::UDWCharacterAttackPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWCharacterAttackPoint::CanHitTarget(AActor* InTarget) const
{
	return Super::CanHitTarget(InTarget);
}

void UDWCharacterAttackPoint::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	Super::OnHitTarget(InTarget, InHitResult);

	if(GetOwnerCharacter()->IsA<ADWMonsterCharacter>())
	{
		const auto& CharacterData = GetOwnerCharacter()->GetCharacterData<UDWMonsterCharacterData>();
		const FVector HitLocation = GetComponentLocation();
		UAudioModuleBPLibrary::PlaySoundAtLocation(InTarget->Implements<UAbilityVitalityInterface>() ? CharacterData.AttackHitSound : CharacterData.AttackMissSound, HitLocation);
		UGameplayStatics::SpawnEmitterAtLocation(this, InTarget->Implements<UAbilityVitalityInterface>() ? CharacterData.AttackHitEffect : CharacterData.AttackMissEffect, HitLocation);
	}
}

void UDWCharacterAttackPoint::ClearHitTargets()
{
	Super::ClearHitTargets();
}

void UDWCharacterAttackPoint::SetHitAble(bool bValue)
{
	Super::SetHitAble(bValue);
}
