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
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Voxel/Datas/VoxelData.h"

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

	GenerateVoxelEntity = nullptr;
	AuxiliaryVoxelEntity = nullptr;
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

	UObjectPoolModuleBPLibrary::DespawnObject(GenerateVoxelEntity);
	UObjectPoolModuleBPLibrary::DespawnObject(AuxiliaryVoxelEntity);
	GenerateVoxelEntity = nullptr;
	AuxiliaryVoxelEntity = nullptr;
}

// Called every frame
void ADWHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWHumanCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);

	if(InItem.IsValid() && InItem.GetType() == EAbilityItemType::Voxel)
	{
		if(!GenerateVoxelEntity)
		{
			GenerateVoxelEntity = UObjectPoolModuleBPLibrary::SpawnObject<AVoxelEntity>();
			GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Creating);
			GenerateVoxelEntity->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GenerateVoxelMesh"));
			GenerateVoxelEntity->SetActorScale3D(FVector(0.3f));
			GenerateVoxelEntity->GetMeshComponent()->SetCastShadow(false);
			GenerateVoxelEntity->GetMeshComponent()->OffsetScale = FVector(0.f, 0.f, 1.f);
			GenerateVoxelEntity->GetMeshComponent()->CenterOffset = FVector(0.f, 0.f, 0.5f);
		}
		GenerateVoxelEntity->Initialize(InItem.ID);
	}
	else if(GenerateVoxelEntity)
	{
		GenerateVoxelEntity->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UObjectPoolModuleBPLibrary::DespawnObject(GenerateVoxelEntity);
		GenerateVoxelEntity = nullptr;
	}
}

void ADWHumanCharacter::OnAuxiliaryItem(const FAbilityItem& InItem)
{
	Super::OnAuxiliaryItem(InItem);

	if(InItem.IsValid() && InItem.GetType() == EAbilityItemType::Voxel && InItem.GetData<UVoxelData>().VoxelType == EVoxelType::Torch)
	{
		if(!AuxiliaryVoxelEntity)
		{
			AuxiliaryVoxelEntity = UObjectPoolModuleBPLibrary::SpawnObject<AVoxelEntity>();
			AuxiliaryVoxelEntity->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AuxiliaryVoxelMesh"));
			AuxiliaryVoxelEntity->SetActorScale3D(FVector(0.3f));
			AuxiliaryVoxelEntity->GetMeshComponent()->SetCastShadow(false);
		}
		AuxiliaryVoxelEntity->Initialize(InItem.ID);
	}
	else if(AuxiliaryVoxelEntity)
	{
		AuxiliaryVoxelEntity->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UObjectPoolModuleBPLibrary::DespawnObject(AuxiliaryVoxelEntity);
		AuxiliaryVoxelEntity = nullptr;
	}
}

void ADWHumanCharacter::RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem)
{
	AAbilityEquipBase* Equip = GetEquip(InPartType);
	if (InItem.IsValid())
	{
		if(Equip && !Equip->GetItemData().EqualID(InItem.ID))
		{
			Equip->OnDischarge();
			UObjectPoolModuleBPLibrary::DespawnObject(Equip);
			Equip = nullptr;
		}
		if(!Equip)
		{
			Equip = UObjectPoolModuleBPLibrary::SpawnObject<AAbilityEquipBase>(nullptr, InItem.GetData<UAbilityEquipDataBase>().EquipClass);
			Equip->Initialize(this, InItem);
			Equip->OnAssemble();
			Equip->Execute_SetActorVisible(Equip, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Fighting);
			Equips.Emplace(InPartType, Equip);
		}
	}
	else if(Equip)
	{
		Equip->OnDischarge();
		UObjectPoolModuleBPLibrary::DespawnObject(Equip);
		Equips.Remove(InPartType);
	}
}

void ADWHumanCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode(InControlMode);

	if(!Execute_IsVisible(this)) return;
	
	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, false);
			HammerMesh->SetVisibility(false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, true);
			HammerMesh->SetVisibility(true);
			break;
		}
	}
}

void ADWHumanCharacter::SetAttackHitAble(bool bValue)
{
	Super::SetAttackHitAble(bValue);

	if(GetWeapon()) GetWeapon()->Execute_SetHitAble(GetWeapon(), bValue);
}

void ADWHumanCharacter::ClearAttackHitTargets()
{
	Super::ClearAttackHitTargets();

	if(GetWeapon()) GetWeapon()->Execute_ClearHitTargets(GetWeapon());
}
