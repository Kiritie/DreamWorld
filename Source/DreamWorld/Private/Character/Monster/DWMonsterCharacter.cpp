// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/DWMonsterCharacter.h"

#include "Ability/Components/CharacterInteractionComponent.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/DWAIBlackboard.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Ability/Inventory/Inventory.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Item/Prop/DWPropData.h"
#include "Voxel/Datas/VoxelData.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Ability/Components/DWCharacterAttackPointComponent.h"

ADWMonsterCharacter::ADWMonsterCharacter()
{
	AttackPoint = CreateDefaultSubobject<UDWCharacterAttackPointComponent>(TEXT("AttackPoint"));
	AttackPoint->SetupAttachment(GetMesh(), TEXT("AttackPoint"));

	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Ride);
	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Feed);
}

bool ADWMonsterCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EDWInteractAction::Feed:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				const FAbilityItem SelectedItem = InInteractionCharacter->GetInventory()->GetSelectedItem();
				return !IsEnemy(InInteractionCharacter) && SelectedItem.IsValid() && SelectedItem.GetType() == EAbilityItemType::Prop && SelectedItem.GetData<UDWPropData>().PropType == EDWPropType::Food;
			}
			break;
		}
		case EDWInteractAction::Ride:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				return !IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetRidingTarget() != this;
			}
			break;
		}
		case EDWInteractAction::UnRide:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				return !IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetRidingTarget() == this;
			}
			break;
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWMonsterCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassivity);

	if(!bPassivity) return;
	
	switch (InInteractAction)
	{
		case EDWInteractAction::Feed:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				TriggerCharacter->GetInventory()->SetConnectInventory(GetInventory());
				TriggerCharacter->GetInventory()->GetSelectedSlot()->MoveItem(1);
				TriggerCharacter->GetInventory()->SetConnectInventory(nullptr);
			}
			break;
		}
		case EDWInteractAction::Ride:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				TriggerCharacter->Ride(this);
			}
			break;
		}
		case EDWInteractAction::UnRide:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				TriggerCharacter->UnRide();
			}
			break;
		}
		default: break;
	}
}

void ADWMonsterCharacter::SetAttackHitAble(bool bValue)
{
	Super::SetAttackHitAble(bValue);

	for(auto Iter : GetAttackPoints())
	{
		Iter->Execute_SetHitAble(Iter, bValue);
	}
}

void ADWMonsterCharacter::ClearAttackHitTargets()
{
	Super::ClearAttackHitTargets();

	for(auto Iter : GetAttackPoints())
	{
		Iter->Execute_ClearHitTargets(Iter);
	}
}

TArray<UDWCharacterAttackPointComponent*> ADWMonsterCharacter::GetAttackPoints()
{
	TArray<UDWCharacterAttackPointComponent*> AttackPoints;
	GetComponents(AttackPoints);
	return AttackPoints;
}
