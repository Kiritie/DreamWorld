// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DWProjectileRemote.h"
#include "Audio/AudioModuleStatics.h"
#include "Kismet/GameplayStatics.h"

ADWProjectileRemote::ADWProjectileRemote()
{
	AttackHitSound = nullptr;
	AttackHitEffect = nullptr;
}

void ADWProjectileRemote::Initialize_Implementation(AActor* InOwnerActor, const FGameplayAbilitySpecHandle& InAbilityHandle)
{
	Super::Initialize_Implementation(InOwnerActor, InAbilityHandle);
}

void ADWProjectileRemote::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	const FVector HitLocation = GetActorLocation();

	Super::OnHitTarget(InTarget, InHitResult);

	UAudioModuleStatics::PlaySoundAtLocation(AttackHitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackHitEffect, HitLocation);
}
