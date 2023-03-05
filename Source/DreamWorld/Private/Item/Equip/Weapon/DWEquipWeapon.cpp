// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Components/BoxComponent.h"
#include "Ability/Character/AbilityCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Ability/Vitality/AbilityVitalityInterface.h>
#include <Audio/AudioModuleBPLibrary.h>
#include "Item/Equip/Weapon/DWEquipWeaponData.h"
#include <Kismet/GameplayStatics.h>

ADWEquipWeapon::ADWEquipWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocationAndRotation(FVector(40, 0, 0), FRotator(0, 0, 0));
	BoxComponent->SetRelativeScale3D(FVector(1.5625f, 1, 0.316406f));
	BoxComponent->SetBoxExtent(FVector(24, 10, 32));
	BoxComponent->SetCollisionProfileName(TEXT("Weapon"));
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADWEquipWeapon::OnBeginOverlap);

	HitTargets = TArray<AActor*>();
}

void ADWEquipWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Execute_CanHitTarget(this, OtherActor))
	{
		Execute_OnHitTarget(this, OtherActor, SweepResult);
	}
}

void ADWEquipWeapon::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	Execute_SetHitAble(this, false);
	Execute_ClearHitTargets(this);
}

bool ADWEquipWeapon::CanHitTarget_Implementation(AActor* InTarget)
{
	return InTarget != GetOwnerCharacter() && !HitTargets.Contains(InTarget);
}

void ADWEquipWeapon::OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult)
{
	HitTargets.Add(InTarget);
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwnerCharacter();
	EventData.Target = InTarget;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerCharacter(), FGameplayTag::RequestGameplayTag("Event.Hit.Attack"), EventData);

	const auto& WeaponData = GetItemData<UDWEquipWeaponData>();
	const FVector HitLocation = MeshComponent->GetSocketLocation(FName("HitPoint"));
	UAudioModuleBPLibrary::PlaySoundAtLocation(InTarget->Implements<UAbilityVitalityInterface>() ? WeaponData.AttackHitSound : WeaponData.AttackMissSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, InTarget->Implements<UAbilityVitalityInterface>() ? WeaponData.AttackHitEffect : WeaponData.AttackMissEffect, HitLocation);
}

void ADWEquipWeapon::ClearHitTargets_Implementation()
{
	HitTargets.Empty();
}

void ADWEquipWeapon::SetHitAble_Implementation(bool bValue)
{
	BoxComponent->SetGenerateOverlapEvents(bValue);
}
