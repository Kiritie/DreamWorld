// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/DWMonsterCharacter.h"

#include "Components/BoxComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/DWAIBlackboard.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Interaction/Components/CharacterInteractionComponent.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"

ADWMonsterCharacter::ADWMonsterCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackPoint"));
	AttackPoint->SetupAttachment(GetMesh(), TEXT("AttackPoint"));
	AttackPoint->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	AttackPoint->SetRelativeScale3D(FVector(1, 1, 1));
	AttackPoint->SetBoxExtent(FVector(20, 20, 20));
	AttackPoint->SetCollisionProfileName(TEXT("DW_Weapon"));
	AttackPoint->SetGenerateOverlapEvents(false);
	AttackPoint->OnComponentBeginOverlap.AddDynamic(this, &ADWMonsterCharacter::OnAttackPointOverlap);

	Interaction->AddInteractionAction(EInteractAction::Ride);
	Interaction->AddInteractionAction(EInteractAction::Feed);
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
	OnHitTarget(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADWMonsterCharacter::SetDamaging(bool bInDamaging)
{
	Super::SetDamaging(bInDamaging);

	AttackPoint->SetGenerateOverlapEvents(bInDamaging);
}

bool ADWMonsterCharacter::CanInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction)
{
	if(!Super::CanInteract(InInteractionTarget, InInteractAction)) return false;
	
	switch (InInteractAction)
	{
		case EInteractAction::Feed:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				if(!IsEnemy(InInteractionCharacter) && UDWHelper::LoadPropData(InInteractionCharacter->GetInventory()->GetSelectedItem().ID).PropType == EPropType::Food)
				{
					return true;
				}
			}
			break;
		}
		case EInteractAction::Ride:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->IsA(ADWHumanCharacter::StaticClass()) && InInteractionCharacter->GetRidingTarget() != this)
				{
					return true;
				}
			}
			break;
		}
		case EInteractAction::UnRide:
		{
			if(ADWCharacter* InInteractionCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				if(!IsEnemy(InInteractionCharacter) && InInteractionCharacter->IsA(ADWHumanCharacter::StaticClass()) && InInteractionCharacter->GetRidingTarget() == this)
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

void ADWMonsterCharacter::OnInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction)
{
	Super::OnInteract(InInteractionTarget, InInteractAction);

	switch (InInteractAction)
	{
		case EInteractAction::Feed:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				TriggerCharacter->GetInventory()->SetConnectInventory(GetInventory());
				TriggerCharacter->GetInventory()->GetSelectedSlot()->MoveItem(1);
				TriggerCharacter->GetInventory()->SetConnectInventory(nullptr);
			}
			break;
		}
		case EInteractAction::Ride:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				TriggerCharacter->Ride(this);
			}
			break;
		}
		case EInteractAction::UnRide:
		{
			if(ADWCharacter* TriggerCharacter = Cast<ADWCharacter>(InInteractionTarget))
			{
				TriggerCharacter->UnRide();
			}
			break;
		}
		default: break;
	}
}
