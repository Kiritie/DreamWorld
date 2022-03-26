// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Components/VitalityInteractionComponent.h"

#include "Character/DWCharacter.h"
#include "Interaction/InteractionInterface.h"

UAbilityVitalityInteractionComponent::UAbilityVitalityInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAbilityVitalityInteractionComponent::DoInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction)
{
	if(!Super::DoInteract(InInteractionTarget, InInteractAction)) return false;

	switch (InInteractAction)
	{
		case EInteractAction::Dialogue:
		{
			return true;
		}
		default: break;
	}
	return false;
}
