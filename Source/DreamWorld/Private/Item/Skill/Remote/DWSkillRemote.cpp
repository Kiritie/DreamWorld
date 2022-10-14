// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Skill/Remote/DWSkillRemote.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Skill/Remote/DWSkillRemoteData.h"

ADWSkillRemote::ADWSkillRemote()
{

}

void ADWSkillRemote::Initialize_Implementation(AAbilityCharacterBase* InOwnerCharacter, const FAbilityItem& InItem)
{
	Super::Initialize_Implementation(InOwnerCharacter, InItem);
}

void ADWSkillRemote::OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult)
{
	const FVector HitLocation = GetActorLocation();

	Super::OnHitTarget_Implementation(InTarget, InHitResult);

	const auto& SkillData = GetItemData<UDWSkillRemoteData>();
	UAudioModuleBPLibrary::PlaySoundAtLocation(SkillData.AttackHitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillData.AttackHitEffect, HitLocation);
}
