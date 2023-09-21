// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Character/DWCharacterAttackPoint.h"
#include "Character/Monster/DWMonsterCharacterData.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Monster/DWMonsterCharacter.h"

UDWCharacterAttackPoint::UDWCharacterAttackPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWCharacterAttackPoint::CanHitTarget_Implementation(AActor* InTarget)
{
	return Super::CanHitTarget_Implementation(InTarget);
}

void UDWCharacterAttackPoint::OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult)
{
	Super::OnHitTarget_Implementation(InTarget, InHitResult);

	if(GetOwnerCharacter()->IsA<ADWMonsterCharacter>())
	{
		const auto& CharacterData = GetOwnerCharacter()->GetCharacterData<UDWMonsterCharacterData>();
		const FVector HitLocation = GetComponentLocation();
		UAudioModuleBPLibrary::PlaySoundAtLocation(InTarget->Implements<UAbilityVitalityInterface>() ? CharacterData.AttackHitSound : CharacterData.AttackMissSound, HitLocation);
		UGameplayStatics::SpawnEmitterAtLocation(this, InTarget->Implements<UAbilityVitalityInterface>() ? CharacterData.AttackHitEffect : CharacterData.AttackMissEffect, HitLocation);
	}
}

void UDWCharacterAttackPoint::ClearHitTargets_Implementation()
{
	Super::ClearHitTargets_Implementation();
}

void UDWCharacterAttackPoint::SetHitAble_Implementation(bool bValue)
{
	Super::SetHitAble_Implementation(bValue);
}
