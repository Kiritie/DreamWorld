// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Components/CharacterInteractionComponent.h"

#include "Character/DWCharacter.h"
#include "Interaction/InteractionInterface.h"

UCharacterInteractionComponent::UCharacterInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UCharacterInteractionComponent::DoInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction)
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
