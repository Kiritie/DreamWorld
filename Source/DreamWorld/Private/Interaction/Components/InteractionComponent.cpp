// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Components/InteractionComponent.h"

#include "Character/DWCharacter.h"
#include "Interaction/InteractionInterface.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"

UInteractionComponent::UInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	UPrimitiveComponent::SetCollisionProfileName(TEXT("DW_Interaction"));
	InitBoxExtent(FVector(100, 100, 50));
	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnEndOverlap);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!GetInteractionOwner() || OtherActor == GetOwner()) return;

	if(OverlappingTarget) return;
	
	if(IInteractionInterface* OtherInteraction = Cast<IInteractionInterface>(OtherActor))
	{
		OverlappingTarget = OtherInteraction;
		if(ADWCharacter* OtherCharacter = Cast<ADWCharacter>(OtherActor))
		{
			if(OtherCharacter->IsPlayer())
			{
				UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
			}
		}
	}
}

void UInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!GetInteractionOwner() || OtherActor == GetOwner()) return;

	if(!OverlappingTarget) return;

	if(IInteractionInterface* OtherInteraction = Cast<IInteractionInterface>(OtherActor))
	{
		if(OverlappingTarget == OtherInteraction)
		{
			OverlappingTarget = nullptr;
			if(ADWCharacter* OtherCharacter = Cast<ADWCharacter>(OtherActor))
			{
				if(OtherCharacter->IsPlayer())
				{
					UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
				}
			}
		}
	}
}

bool UInteractionComponent::DoInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction)
{
	if(!InInteractionTarget || InInteractAction == EInteractAction::None || !InteractActions.Contains(InInteractAction) || InteractingAction != EInteractAction::None) return false;
	
	if(GetInteractionOwner()->CanInteract(InInteractionTarget, InInteractAction))
	{
		SetInteractionTarget(InInteractionTarget);
		GetInteractionOwner()->OnInteract(InInteractionTarget, InInteractAction);
	}
	return false;
}

void UInteractionComponent::AddInteractionAction(EInteractAction InInteractAction)
{
	if(!InteractActions.Contains(InInteractAction))
	{
		InteractActions.Add(InInteractAction);
	}
}

void UInteractionComponent::RemoveInteractionAction(EInteractAction InInteractAction)
{
	if(InteractActions.Contains(InInteractAction))
	{
		InteractActions.Remove(InInteractAction);
	}
}

TArray<EInteractAction> UInteractionComponent::GetValidInteractActions(IInteractionInterface* InInteractionTarget) const
{
	TArray<EInteractAction> ReturnValues;
	for(auto Iter : InteractActions)
	{
		if(GetInteractionOwner()->CanInteract(InInteractionTarget, Iter))
		{
			ReturnValues.Add(Iter);
		}
	}
	return ReturnValues;
}

IInteractionInterface* UInteractionComponent::GetInteractionOwner() const
{
	return Cast<IInteractionInterface>(GetOwner());
}

void UInteractionComponent::SetInteractionTarget(IInteractionInterface* InInteractionTarget)
{
	InteractionTarget = InInteractionTarget;
	if(InteractionTarget)
	{
		InteractionTarget->GetInteractionComponent()->InteractionTarget = GetInteractionOwner();
	}
}
