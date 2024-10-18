// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/DWMonsterCharacter.h"

#include "Ability/Character/DWCharacterAttackPoint.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Item/Prop/DWPropData.h"

ADWMonsterCharacter::ADWMonsterCharacter()
{
	AttackPoint = CreateDefaultSubobject<UDWCharacterAttackPoint>(TEXT("AttackPoint"));
	AttackPoint->SetupAttachment(GetMesh(), TEXT("AttackPoint"));

	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Ride);
	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::UnRide);
	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Feed);
}

bool ADWMonsterCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case (EInteractAction)EDWInteractAction::Feed:
		{
			if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				const FAbilityItem SelectedItem = InteractionCharacter->GetInventory()->GetSelectedItem(ESlotSplitType::Shortcut);
				return !IsEnemy(InteractionCharacter) && SelectedItem.IsValid() && SelectedItem.GetType() == EAbilityItemType::Prop && SelectedItem.GetData<UDWPropData>().PropType == EDWPropType::Food;
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::Ride:
		{
			if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				return !IsEnemy(InteractionCharacter) && InteractionCharacter->GetRidingTarget() != this;
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::UnRide:
		{
			if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				return !IsEnemy(InteractionCharacter) && InteractionCharacter->GetRidingTarget() == this;
			}
			break;
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWMonsterCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(bPassive)
	{
		switch (InInteractAction)
		{
			case (EInteractAction)EDWInteractAction::Feed:
			{
				if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
				{
					InteractionCharacter->GetInventory()->SetConnectInventory(GetInventory());
					InteractionCharacter->GetInventory()->GetSelectedSlot(ESlotSplitType::Shortcut)->MoveItem(1);
					InteractionCharacter->GetInventory()->SetConnectInventory(nullptr);
				}
				break;
			}
			case (EInteractAction)EDWInteractAction::Ride:
			{
				if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
				{
					InteractionCharacter->Ride(this);
				}
				break;
			}
			case (EInteractAction)EDWInteractAction::UnRide:
			{
				if(ADWCharacter* InteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
				{
					InteractionCharacter->UnRide();
				}
				break;
			}
			default: break;
		}
	}
}

void ADWMonsterCharacter::SetAttackHitAble(bool bValue)
{
	Super::SetAttackHitAble(bValue);

	for(auto Iter : GetAttackPoints())
	{
		Iter->SetHitAble(bValue);
	}
}

void ADWMonsterCharacter::ClearAttackHitTargets()
{
	Super::ClearAttackHitTargets();

	for(auto Iter : GetAttackPoints())
	{
		Iter->ClearHitTargets();
	}
}

TArray<UDWCharacterAttackPoint*> ADWMonsterCharacter::GetAttackPoints()
{
	TArray<UDWCharacterAttackPoint*> AttackPoints;
	GetComponents(AttackPoints);
	return AttackPoints;
}
