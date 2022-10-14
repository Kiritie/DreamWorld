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

	Interaction->AddInteractionAction((EInteractAction)EDWInteractAction::Ride);
	Interaction->AddInteractionAction((EInteractAction)EDWInteractAction::Feed);
}

// Called when the game starts or when spawned
void ADWMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADWMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADWMonsterCharacter::CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	if(!Super::CanInteract(InInteractionAgent, InInteractAction)) return false;
	
	switch (InInteractAction)
	{
		case EDWInteractAction::Feed:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetInventory()->GetSelectedItem().GetData<UDWPropData>().PropType == EDWPropType::Food)
				{
					return true;
				}
			}
			break;
		}
		case EDWInteractAction::Ride:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetRidingTarget() != this)
				{
					return true;
				}
			}
			break;
		}
		case EDWInteractAction::UnRide:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionAgent))
			{
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->GetRidingTarget() == this)
				{
					return true;
				}
			}
			break;
		}
		default: return true;
	}
	return false;
}

void ADWMonsterCharacter::OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	Super::OnInteract(InInteractionAgent, InInteractAction);

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
