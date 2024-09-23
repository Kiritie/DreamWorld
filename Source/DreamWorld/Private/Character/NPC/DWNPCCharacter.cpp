// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/DWNPCCharacter.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Inventory/WidgetInventoryBox.h"

ADWNPCCharacter::ADWNPCCharacter()
{
	Interaction->AddInteractAction(EInteractAction::Dialogue);
	Interaction->AddInteractAction(EInteractAction::Transaction);
}

bool ADWNPCCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EInteractAction::Dialogue:
		{
			if(ADWPlayerCharacter* InteractionCharacter = Cast<ADWPlayerCharacter>(InInteractionAgent))
			{
				return true;
			}
			break;
		}
		case EInteractAction::Transaction:
		{
			if(ADWPlayerCharacter* InteractionCharacter = Cast<ADWPlayerCharacter>(InInteractionAgent))
			{
				return true;
			}
			break;
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWNPCCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassivity);

	if(bPassivity)
	{
		switch (InInteractAction)
		{
			case EInteractAction::Dialogue:
			{
				if(ADWPlayerCharacter* InteractionCharacter = Cast<ADWPlayerCharacter>(InInteractionAgent))
				{
				}
				break;
			}
			case EInteractAction::Transaction:
			{
				if(ADWPlayerCharacter* InteractionCharacter = Cast<ADWPlayerCharacter>(InInteractionAgent))
				{
					UWidgetModuleStatics::OpenUserWidget<UWidgetInventoryBox>({ this });
					FDelegateHandle DelegateHandle = UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBox>()->OnClosed.AddLambda([this, DelegateHandle](bool bInstant)
					{
						UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBox>()->OnClosed.Remove(DelegateHandle);
					});
				}
				break;
			}
			default: break;
		}
	}
}
