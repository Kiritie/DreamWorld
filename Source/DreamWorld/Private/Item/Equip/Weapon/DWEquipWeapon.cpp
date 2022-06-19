// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/Weapon/DWEquipWeapon.h"

#include "Components/BoxComponent.h"
#include "Character/DWCharacter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADWEquipWeapon::ADWEquipWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocationAndRotation(FVector(40, 0, 0), FRotator(0, 0, 0));
	BoxComponent->SetRelativeScale3D(FVector(1.5625f, 1, 0.316406f));
	BoxComponent->SetBoxExtent(FVector(24, 10, 32));
	BoxComponent->SetCollisionProfileName(TEXT("Weapon"));
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADWEquipWeapon::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ADWEquipWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ADWEquipWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHitTarget(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

// Called every frame
void ADWEquipWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWEquipWeapon::SetCollisionEnable(bool InValue)
{
	BoxComponent->SetGenerateOverlapEvents(InValue);
}
