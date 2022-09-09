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
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"

ADWHumanCharacter::ADWHumanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(69);
	GetCapsuleComponent()->SetCapsuleRadius(24);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -70));

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

	UObjectPoolModuleBPLibrary::DespawnObject(VoxelEntity);
	VoxelEntity = nullptr;
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
			if(VoxelEntity) VoxelEntity->Execute_SetActorVisible(VoxelEntity, false);
			HammerMesh->SetVisibility(false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(VoxelEntity) VoxelEntity->Execute_SetActorVisible(VoxelEntity, true);
			HammerMesh->SetVisibility(true);
			break;
		}
	}
}

void ADWHumanCharacter::SetGenerateVoxelID(const FPrimaryAssetId& InGenerateVoxelID)
{
	Super::SetGenerateVoxelID(InGenerateVoxelID);

	if(InGenerateVoxelID.IsValid())
	{
		if(!VoxelEntity)
		{
			VoxelEntity = UObjectPoolModuleBPLibrary::SpawnObject<AVoxelEntity>();
			VoxelEntity->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VoxelMesh"));
			VoxelEntity->Execute_SetActorVisible(VoxelEntity, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Creating);
			VoxelEntity->GetMeshComponent()->OffsetScale = FVector(0.f, 0.f, 1.f);
			VoxelEntity->GetMeshComponent()->CenterOffset = FVector(0.f, 0.f, 0.5f);
		}
		VoxelEntity->Initialize(InGenerateVoxelID);
	}
	else if(VoxelEntity)
	{
		VoxelEntity->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UObjectPoolModuleBPLibrary::DespawnObject(VoxelEntity);
		VoxelEntity = nullptr;
	}
}

void ADWHumanCharacter::SetAttackDamageAble(bool bInDamaging)
{
	Super::SetAttackDamageAble(bInDamaging);

	if(GetWeapon()) GetWeapon()->SetCollisionEnable(bInDamaging);
}
