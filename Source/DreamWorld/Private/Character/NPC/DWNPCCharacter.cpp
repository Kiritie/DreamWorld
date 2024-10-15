// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/DWNPCCharacter.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Transaction/WidgetTransactionPanel.h"

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

void ADWNPCCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(bPassive)
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
					UWidgetModuleStatics::OpenUserWidget<UWidgetTransactionPanel>({ this });
					FDelegateHandle DelegateHandle = UWidgetModuleStatics::GetUserWidget<UWidgetTransactionPanel>()->OnClosed.AddLambda([this, DelegateHandle](bool bInstant)
					{
						UWidgetModuleStatics::GetUserWidget<UWidgetTransactionPanel>()->OnClosed.Remove(DelegateHandle);
					});
				}
				break;
			}
			default: break;
		}
	}
}
