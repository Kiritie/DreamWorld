// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Human/DWHumanCharacter.h"

#include "Dialogue.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Character/States/AbilityCharacterState_Crouch.h"
#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "Ability/Character/States/AbilityCharacterState_Interrupt.h"
#include "Ability/Character/States/AbilityCharacterState_Jump.h"
#include "Ability/Character/States/AbilityCharacterState_Static.h"
#include "Ability/Character/States/AbilityCharacterState_Swim.h"
#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "Character/Human/States/DWHumanCharacterState_Defend.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Aim.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Death.h"
#include "Character/States/DWCharacterState_Spawn.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "Common/Interaction/InteractionComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "Item/Equip/DWEquipData.h"
#include "Item/Equip/Weapon/DWEquipWeaponMelee.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Dialogue/WidgetDialogueBox.h"
#include "Widget/Interaction/WidgetInteractionBox.h"

ADWHumanCharacter::ADWHumanCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	GetCharacterMovement()->AirControl = 0.3f;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

	GenerateHammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HammerMesh"));
	GenerateHammerMesh->SetupAttachment(GetMesh(), TEXT("GenerateHammerMesh"));
	GenerateHammerMesh->SetRelativeLocation(FVector::ZeroVector);
	GenerateHammerMesh->SetRelativeRotation(FRotator::ZeroRotator);
	GenerateHammerMesh->SetVisibility(false);
	GenerateHammerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FSM->DefaultState = UDWCharacterState_Spawn::StaticClass();
	FSM->FinalState = UDWCharacterState_Death::StaticClass();
	
	FSM->States.Empty();
	FSM->States.Add(UDWCharacterState_Aim::StaticClass());
	FSM->States.Add(UDWCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWCharacterState_Death::StaticClass());
	FSM->States.Add(UDWCharacterState_Spawn::StaticClass());
	FSM->States.Add(UDWHumanCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWCharacterState_Dodge::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Static::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Swim::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Walk::StaticClass());

	Interaction->AddInteractAction(EInteractAction::Dialogue);

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

bool ADWHumanCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EInteractAction::Dialogue:
		{
			if(Cast<ADWPlayerCharacter>(InInteractionAgent))
			{
				if(UWidgetModuleStatics::GetUserWidget<UWidgetDialogueBox>() && !UWidgetModuleStatics::GetUserWidget<UWidgetDialogueBox>()->IsWidgetOpened())
				{
					return true;
				}
			}
			break;
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWHumanCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(bPassive)
	{
		switch (InInteractAction)
		{
			case EInteractAction::Dialogue:
			{
				if(Cast<ADWPlayerCharacter>(InInteractionAgent))
				{
					if(UWidgetDialogueBox* DialogueBox = UWidgetModuleStatics::GetUserWidget<UWidgetDialogueBox>())
					{
						FDelegateHandle DelegateHandle = DialogueBox->OnClosed.AddLambda([this, DelegateHandle, DialogueBox](bool bInstant)
						{
							DialogueBox->OnClosed.Remove(DelegateHandle);
							if(UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>())
							{
								UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>()->Refresh();
							}
						});
						DialogueBox->Open({ Dialogue });
					}
				}
				break;
			}
			default: break;
		}
	}
}

void ADWHumanCharacter::OnPreChangeItem(const FAbilityItem& InOldItem)
{
	Super::OnPreChangeItem(InOldItem);

	if(InOldItem.IsValid() && InOldItem.InventorySlot->IsMatched())
	{
		const auto& ItemData = InOldItem.GetData<UAbilityItemDataBase>();
		switch(ItemData.GetItemType())
		{
			case EAbilityItemType::Equip:
			{
				const auto& EquipData = InOldItem.GetData<UDWEquipData>();
				if(ADWEquip* Equip = GetEquip(EquipData.EquipPart))
				{
					Equip->OnDisassemble();
					UObjectPoolModuleStatics::DespawnObject(Equip);
					Equips.Remove(EquipData.EquipPart);
				}
				break;
			}
			default: break;
		}
	}
}

void ADWHumanCharacter::OnChangeItem(const FAbilityItem& InNewItem)
{
	Super::OnChangeItem(InNewItem);

	if(InNewItem.IsValid() && InNewItem.InventorySlot->IsMatched())
	{
		const auto& ItemData = InNewItem.GetData<UAbilityItemDataBase>();
		switch(ItemData.GetItemType())
		{
			case EAbilityItemType::Equip:
			{
				const auto& EquipData = InNewItem.GetData<UDWEquipData>();
				if(ADWEquip* Equip = Cast<ADWEquip>(UAbilityModuleStatics::SpawnAbilityItem(InNewItem, this)))
				{
					Equip->OnAssemble();
					Equip->Execute_SetActorVisible(Equip, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Fighting);
					Equips.Emplace(EquipData.EquipPart, Equip);
				}
				break;
			}
			default: break;
		}
	}
}

void ADWHumanCharacter::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);
}

void ADWHumanCharacter::OnDeactiveItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnDeactiveItem(InItem, bPassive);
}

void ADWHumanCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);

	if(InItem.InventorySlot->GetSplitType() == ESlotSplitType::Shortcut)
	{
		if(InItem.IsValid() && InItem.GetType() == EAbilityItemType::Voxel)
		{
			if(!GenerateVoxelEntity)
			{
				GenerateVoxelEntity = UObjectPoolModuleStatics::SpawnObject<AVoxelEntity>();
				GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Creating);
				AttachActor(GenerateVoxelEntity, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GenerateVoxelMesh"));
				GenerateVoxelEntity->SetActorScale3D(FVector(0.3f));
			}
			GenerateVoxelEntity->LoadSaveData(new FVoxelItem(InItem));
		}
		else if(GenerateVoxelEntity)
		{
			DetachActor(GenerateVoxelEntity, FDetachmentTransformRules::KeepWorldTransform);
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
			AttachActor(AuxiliaryVoxelEntity, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AuxiliaryVoxelMesh"));
			AuxiliaryVoxelEntity->SetActorScale3D(FVector(0.3f));
			AuxiliaryVoxelEntity->GetMeshComponent()->SetCastShadow(false);
		}
		AuxiliaryVoxelEntity->LoadSaveData(new FVoxelItem(InItem));
	}
	else if(AuxiliaryVoxelEntity)
	{
		DetachActor(AuxiliaryVoxelEntity, FDetachmentTransformRules::KeepWorldTransform);
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

	if(ADWEquipWeaponMelee* Weapon = GetWeapon<ADWEquipWeaponMelee>(AttackWeaponPart))
	{
		Weapon->SetHitAble(bValue);
	}
}

void ADWHumanCharacter::ClearHitTargets()
{
	Super::ClearHitTargets();

	if(ADWEquipWeaponMelee* Weapon = GetWeapon<ADWEquipWeaponMelee>(AttackWeaponPart))
	{
		Weapon->ClearHitTargets();
	}
}
