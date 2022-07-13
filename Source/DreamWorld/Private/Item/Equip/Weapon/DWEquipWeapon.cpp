// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/Weapon/DWEquipWeapon.h"

#include "Components/BoxComponent.h"
#include "Character/DWCharacter.h"
#include "Components/StaticMeshComponent.h"

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
}

void ADWEquipWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHitTarget(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADWEquipWeapon::OnHitTarget_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADWEquipWeapon::SetCollisionEnable(bool InValue)
{
	BoxComponent->SetGenerateOverlapEvents(InValue);
}
