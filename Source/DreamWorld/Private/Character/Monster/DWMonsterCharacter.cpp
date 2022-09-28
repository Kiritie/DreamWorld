// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/DWMonsterCharacter.h"

#include "Ability/Components/CharacterInteractionComponent.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/DWAIBlackboard.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Item/Prop/DWPropData.h"
#include "Voxel/Datas/VoxelData.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"

ADWMonsterCharacter::ADWMonsterCharacter()
{
	AttackPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackPoint"));
	AttackPoint->SetupAttachment(GetMesh(), TEXT("AttackPoint"));
	AttackPoint->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	AttackPoint->SetRelativeScale3D(FVector(1, 1, 1));
	AttackPoint->SetBoxExtent(FVector(20, 20, 20));
	AttackPoint->SetCollisionProfileName(TEXT("Weapon"));
	AttackPoint->SetGenerateOverlapEvents(false);
	AttackPoint->OnComponentBeginOverlap.AddDynamic(this, &ADWMonsterCharacter::OnAttackPointOverlap);

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

void ADWMonsterCharacter::OnAttackPointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != this)
	{
		OnHitTarget(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void ADWMonsterCharacter::SetAttackDamageAble(bool bInDamaging)
{
	Super::SetAttackDamageAble(bInDamaging);

	AttackPoint->SetGenerateOverlapEvents(bInDamaging);
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
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->IsA<ADWHumanCharacter>() && InInteractionCharacter->GetRidingTarget() != this)
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
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->IsA<ADWHumanCharacter>() && InInteractionCharacter->GetRidingTarget() == this)
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
