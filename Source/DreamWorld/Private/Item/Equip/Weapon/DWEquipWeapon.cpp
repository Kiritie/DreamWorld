// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Ability/Vitality/AbilityVitalityInterface.h>
#include <Audio/AudioModuleStatics.h>
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
	if(CanHitTarget(OtherActor))
	{
		OnHitTarget(OtherActor, SweepResult);
	}
}

void ADWEquipWeapon::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	SetHitAble(false);
	ClearHitTargets();
}

bool ADWEquipWeapon::CanHitTarget(AActor* InTarget) const
{
	return InTarget != GetOwnerActor() && !HitTargets.Contains(InTarget);
}

void ADWEquipWeapon::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	HitTargets.Add(InTarget);
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwnerActor();
	EventData.Target = InTarget;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerActor(), FGameplayTag::RequestGameplayTag("Event.Hit.Attack"), EventData);

	const auto& WeaponData = GetItemData<UDWEquipWeaponData>();
	const FVector HitLocation = MeshComponent->GetSocketLocation(FName("HitPoint"));
	UAudioModuleStatics::PlaySoundAtLocation(InTarget->Implements<UAbilityVitalityInterface>() ? WeaponData.AttackHitSound : WeaponData.AttackMissSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, InTarget->Implements<UAbilityVitalityInterface>() ? WeaponData.AttackHitEffect : WeaponData.AttackMissEffect, HitLocation);
}

void ADWEquipWeapon::ClearHitTargets()
{
	HitTargets.Empty();
}

void ADWEquipWeapon::SetHitAble(bool bValue)
{
	BoxComponent->SetGenerateOverlapEvents(bValue);
}
