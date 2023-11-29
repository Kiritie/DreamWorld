// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Human/DWHumanCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Item/Equip/DWEquipData.h"
#include "Voxel/Datas/VoxelData.h"

ADWHumanCharacter::ADWHumanCharacter()
{
	Interaction->SetRelativeLocation(FVector(0.f, 0.f, -19.f));

	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

	GenerateVoxelEntity = nullptr;
	AuxiliaryVoxelEntity = nullptr;
}

void ADWHumanCharacter::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWHumanCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	UObjectPoolModuleStatics::DespawnObject(GenerateVoxelEntity);
	UObjectPoolModuleStatics::DespawnObject(AuxiliaryVoxelEntity);
	GenerateVoxelEntity = nullptr;
	AuxiliaryVoxelEntity = nullptr;
}

void ADWHumanCharacter::OnAssembleItem(const FAbilityItem& InItem)
{
	Super::OnAssembleItem(InItem);

	const auto& ItemData = InItem.GetData<UAbilityItemDataBase>();
	switch(ItemData.GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			const auto& EquipData = InItem.GetData<UDWEquipData>();
			if(AAbilityEquipBase* Equip = UObjectPoolModuleStatics::SpawnObject<AAbilityEquipBase>(nullptr, nullptr, InItem.GetData<UAbilityEquipDataBase>().EquipClass))
			{
				Equip->Initialize(this, InItem);
				Equip->OnAssemble();
				Equip->Execute_SetActorVisible(Equip, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Fighting);
				Equips.Emplace(EquipData.PartType, Equip);
			}
			break;
		}
		default: break;
	}
}

void ADWHumanCharacter::OnDischargeItem(const FAbilityItem& InItem)
{
	Super::OnDischargeItem(InItem);

	const auto& ItemData = InItem.GetData<UAbilityItemDataBase>();
	switch(ItemData.GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			const auto& EquipData = InItem.GetData<UDWEquipData>();
			if(AAbilityEquipBase* Equip = GetEquip(EquipData.PartType))
			{
				Equip->OnDischarge();
				UObjectPoolModuleStatics::DespawnObject(Equip);
				Equips.Emplace(EquipData.PartType, nullptr);
			}
			break;
		}
		default: break;
	}
}

void ADWHumanCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);

	if(InItem.IsValid() && InItem.GetType() == EAbilityItemType::Voxel)
	{
		if(!GenerateVoxelEntity)
		{
			GenerateVoxelEntity = UObjectPoolModuleStatics::SpawnObject<AVoxelEntity>();
			GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Creating);
			GenerateVoxelEntity->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GenerateVoxelMesh"));
			GenerateVoxelEntity->SetActorScale3D(FVector(0.3f));
		}
		GenerateVoxelEntity->LoadSaveData(new FVoxelItem(InItem));
	}
	else if(GenerateVoxelEntity)
	{
		GenerateVoxelEntity->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UObjectPoolModuleStatics::DespawnObject(GenerateVoxelEntity);
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
			AuxiliaryVoxelEntity = UObjectPoolModuleStatics::SpawnObject<AVoxelEntity>();
			AuxiliaryVoxelEntity->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AuxiliaryVoxelMesh"));
			AuxiliaryVoxelEntity->SetActorScale3D(FVector(0.3f));
			AuxiliaryVoxelEntity->GetMeshComponent()->SetCastShadow(false);
		}
		AuxiliaryVoxelEntity->LoadSaveData(new FVoxelItem(InItem));
	}
	else if(AuxiliaryVoxelEntity)
	{
		AuxiliaryVoxelEntity->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UObjectPoolModuleStatics::DespawnObject(AuxiliaryVoxelEntity);
		AuxiliaryVoxelEntity = nullptr;
	}
}

void ADWHumanCharacter::SetControlMode_Implementation(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode_Implementation(InControlMode);

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, Execute_IsVisible(this));
			break;
		}
	}
}

void ADWHumanCharacter::SetAttackHitAble(bool bValue)
{
	Super::SetAttackHitAble(bValue);

	if(GetWeapon()) GetWeapon()->SetHitAble(bValue);
}

void ADWHumanCharacter::ClearAttackHitTargets()
{
	Super::ClearAttackHitTargets();

	if(GetWeapon()) GetWeapon()->ClearHitTargets();
}
