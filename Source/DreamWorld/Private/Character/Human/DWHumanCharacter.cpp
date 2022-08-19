// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Human/DWHumanCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Human/DWHumanCharacterAnim.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/DWAIBlackboard.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"

ADWHumanCharacter::ADWHumanCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(69);
	GetCapsuleComponent()->SetCapsuleRadius(24);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -70));
	
	VoxelMesh = CreateDefaultSubobject<UChildActorComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(GetMesh(), FName("VoxelMesh"));
	VoxelMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	VoxelMesh->SetChildActorClass(AVoxelEntity::StaticClass());

	HammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("HammerMesh"));
	HammerMesh->SetupAttachment(GetMesh(), FName("HammerMesh"));
	HammerMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	HammerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HammerMesh->SetCastShadow(false);
	HammerMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ADWHumanCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADWHumanCharacter::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWHumanCharacter::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
	
	if(AVoxelEntity* VoxelEntity = Cast<AVoxelEntity>(VoxelMesh->GetChildActor()))
	{
		VoxelEntity->Initialize(FPrimaryAssetId());
	}
}

// Called every frame
void ADWHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWHumanCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode(InControlMode);

	if(!Execute_IsVisible(this)) return;
	
	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			VoxelMesh->SetVisibility(false);
			HammerMesh->SetVisibility(false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			VoxelMesh->SetVisibility(true);
			HammerMesh->SetVisibility(true);
			break;
		}
	}
}

void ADWHumanCharacter::SetGenerateVoxelItem(const FVoxelItem& InGenerateVoxelItem)
{
	Super::SetGenerateVoxelItem(InGenerateVoxelItem);

	if(AVoxelEntity* VoxelEntity = Cast<AVoxelEntity>(VoxelMesh->GetChildActor()))
	{
		VoxelEntity->Initialize(GenerateVoxelItem.ID);
	}
}

void ADWHumanCharacter::SetAttackDamageAble(bool bInDamaging)
{
	Super::SetAttackDamageAble(bInDamaging);

	if(GetWeapon()) GetWeapon()->SetCollisionEnable(bInDamaging);
}
