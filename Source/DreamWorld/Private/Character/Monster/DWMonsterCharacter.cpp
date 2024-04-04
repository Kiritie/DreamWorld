// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/DWMonsterCharacter.h"

#include "Ability/Character/DWCharacterAttackPoint.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Item/Prop/DWPropData.h"

ADWMonsterCharacter::ADWMonsterCharacter()
{
	AttackPoint = CreateDefaultSubobject<UDWCharacterAttackPoint>(TEXT("AttackPoint"));
	AttackPoint->SetupAttachment(GetMesh(), TEXT("AttackPoint"));

	Interaction->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Ride);
	Interaction->AddInteractAction((EInteractAction)EDWInteractAction::Feed);
}

bool ADWMonsterCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case (EInteractAction)EDWInteractAction::Feed:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				const FAbilityItem SelectedItem = InInteractionCharacter->GetInventory()->GetSelectedItem();
				return !IsEnemy(InInteractionCharacter) && SelectedItem.IsValid() && SelectedItem.GetType() == EAbilityItemType::Prop && SelectedItem.GetData<UDWPropData>().PropType == EDWPropType::Food;
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::Ride:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				return !IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetRidingTarget() != this;
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::UnRide:
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
		case (EInteractAction)EDWInteractAction::Feed:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				TriggerCharacter->GetInventory()->SetConnectInventory(GetInventory());
				TriggerCharacter->GetInventory()->GetSelectedSlot()->MoveItem(1);
				TriggerCharacter->GetInventory()->SetConnectInventory(nullptr);
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::Ride:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				TriggerCharacter->Ride(this);
			}
			break;
		}
		case (EInteractAction)EDWInteractAction::UnRide:
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
