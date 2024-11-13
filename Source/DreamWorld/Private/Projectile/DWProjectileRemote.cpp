// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DWProjectileRemote.h"
#include "Audio/AudioModuleStatics.h"
#include "Kismet/GameplayStatics.h"

ADWProjectileRemote::ADWProjectileRemote()
{
	AttackHitSound = nullptr;
	AttackHitEffect = nullptr;
}

void ADWProjectileRemote::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	const FVector HitLocation = GetActorLocation();
	UAudioModuleStatics::PlaySoundAtLocation(AttackHitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackHitEffect, HitLocation);

	Super::OnHitTarget(InTarget, InHitResult);
}
