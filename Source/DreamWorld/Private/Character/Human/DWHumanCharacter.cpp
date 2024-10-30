// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Human/DWHumanCharacter.h"

#include "Ability/AbilityModuleStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "Character/Human/States/DWHumanCharacterState_Defend.h"
#include "Character/States/DWCharacterState_Aim.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Crouch.h"
#include "Character/States/DWCharacterState_Death.h"
#include "Character/States/DWCharacterState_Spawn.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Character/States/DWCharacterState_Fall.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Interrupt.h"
#include "Character/States/DWCharacterState_Jump.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "Character/States/DWCharacterState_Static.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "Item/Equip/DWEquipData.h"
#include "Item/Equip/Weapon/DWEquipWeaponMelee.h"
#include "Voxel/Datas/VoxelData.h"

ADWHumanCharacter::ADWHumanCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

	GenerateHammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HammerMesh"));
	GenerateHammerMesh->SetupAttachment(GetMesh(), TEXT("HammerMesh"));
	GenerateHammerMesh->SetRelativeLocation(FVector::ZeroVector);
	GenerateHammerMesh->SetRelativeRotation(FRotator::ZeroRotator);
	GenerateHammerMesh->SetVisibility(false);
	
	FSM->DefaultState = UDWCharacterState_Spawn::StaticClass();
	FSM->FinalState = UDWCharacterState_Death::StaticClass();
	
	FSM->States.Empty();
	FSM->States.Add(UDWCharacterState_Aim::StaticClass());
	FSM->States.Add(UDWCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UDWCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWCharacterState_Death::StaticClass());
	FSM->States.Add(UDWCharacterState_Spawn::StaticClass());
	FSM->States.Add(UDWHumanCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWCharacterState_Dodge::StaticClass());
	FSM->States.Add(UDWCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UDWCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UDWCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UDWCharacterState_Static::StaticClass());
	FSM->States.Add(UDWCharacterState_Swim::StaticClass());
	FSM->States.Add(UDWCharacterState_Walk::StaticClass());

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

void ADWHumanCharacter::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);

	const auto& ItemData = InItem.GetData<UAbilityItemDataBase>();
	switch(ItemData.GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			const auto& EquipData = InItem.GetData<UDWEquipData>();
			if(AAbilityEquipBase* Equip = Cast<AAbilityEquipBase>(UAbilityModuleStatics::SpawnAbilityItem(InItem, this)))
			{
				Equip->OnAssemble();
				Equip->Execute_SetActorVisible(Equip, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Fighting);
				Equips.Emplace(EquipData.PartType, Equip);
			}
			break;
		}
		default: break;
	}
}

void ADWHumanCharacter::OnDeactiveItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnDeactiveItem(InItem, bPassive);

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

void ADWHumanCharacter::OnSelectItem(ESlotSplitType InSplitType, const FAbilityItem& InItem)
{
	Super::OnSelectItem(InSplitType, InItem);

	if(InSplitType == ESlotSplitType::Shortcut)
	{
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
			GenerateHammerMesh->SetVisibility(false);
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			GenerateHammerMesh->SetVisibility(Execute_IsVisible(this));
			if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, Execute_IsVisible(this));
			break;
		}
	}
}

void ADWHumanCharacter::SetHitAble(bool bValue)
{
	Super::SetHitAble(bValue);

	if(ADWEquipWeaponMelee* Weapon = GetWeapon<ADWEquipWeaponMelee>())
	{
		Weapon->SetHitAble(bValue);
	}
}

void ADWHumanCharacter::ClearHitTargets()
{
	Super::ClearHitTargets();

	if(ADWEquipWeaponMelee* Weapon = GetWeapon<ADWEquipWeaponMelee>())
	{
		Weapon->ClearHitTargets();
	}
}
