// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/Weapon/DWEquipWeaponMelee.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Audio/AudioModuleStatics.h>
#include "Kismet/GameplayStatics.h"

ADWEquipWeaponMelee::ADWEquipWeaponMelee()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(20.f));
	BoxComponent->SetCollisionProfileName(TEXT("Weapon"));
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADWEquipWeaponMelee::OnBeginOverlap);
	
	AttackHitSound = nullptr;
	AttackHitEffect = nullptr;

	HitTargets = TArray<AActor*>();
}

void ADWEquipWeaponMelee::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWEquipWeaponMelee::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	SetHitAble(false);
	ClearHitTargets();
}

void ADWEquipWeaponMelee::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(CanHitTarget(OtherActor))
	{
		OnHitTarget(OtherActor, SweepResult);
	}
}

bool ADWEquipWeaponMelee::CanHitTarget(AActor* InTarget) const
{
	return InTarget != GetOwnerActor() && !HitTargets.Contains(InTarget);
}

void ADWEquipWeaponMelee::OnHitTarget(AActor* InTarget, const FHitResult& InHitResult)
{
	HitTargets.Add(InTarget);
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwnerActor();
	EventData.Target = InTarget;
	EventData.OptionalObject = this;
	EventData.ContextHandle.AddHitResult(InHitResult);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerActor(), GameplayTags::Event_Hit_Attack, EventData);

	const FVector HitLocation = GetMeshComponent()->GetSocketLocation(FName("HitPoint"));
	UAudioModuleStatics::PlaySoundAtLocation(AttackHitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackHitEffect, HitLocation);
}

bool ADWEquipWeaponMelee::IsHitAble() const
{
	return BoxComponent->GetGenerateOverlapEvents();
}

void ADWEquipWeaponMelee::SetHitAble(bool bValue)
{
	BoxComponent->SetGenerateOverlapEvents(bValue);
}

void ADWEquipWeaponMelee::ClearHitTargets()
{
	HitTargets.Empty();
}

TArray<AActor*> ADWEquipWeaponMelee::GetHitTargets() const
{
	return HitTargets;
}
