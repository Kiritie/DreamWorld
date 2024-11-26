// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/DWNPCCharacter.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Interaction/WidgetInteractionBox.h"
#include "Widget/Transaction/WidgetTransactionPanel.h"

ADWNPCCharacter::ADWNPCCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Interaction->AddInteractAction(EInteractAction::Transaction);
}

bool ADWNPCCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EInteractAction::Transaction:
		{
			if(Cast<ADWPlayerCharacter>(InInteractionAgent))
			{
				if(UWidgetModuleStatics::GetUserWidget<UWidgetTransactionPanel>() && !UWidgetModuleStatics::GetUserWidget<UWidgetTransactionPanel>()->IsWidgetOpened())
				{
					return true;
				}
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
			case EInteractAction::Transaction:
			{
				if(Cast<ADWPlayerCharacter>(InInteractionAgent))
				{
					if(UWidgetTransactionPanel* TransactionPanel = UWidgetModuleStatics::GetUserWidget<UWidgetTransactionPanel>())
					{
						FDelegateHandle DelegateHandle = TransactionPanel->OnClosed.AddLambda([this, DelegateHandle, TransactionPanel](bool bInstant)
						{
							TransactionPanel->OnClosed.Remove(DelegateHandle);
							if(UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>())
							{
								UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>()->Refresh();
							}
						});
						TransactionPanel->Open({ this });
					}
				}
				break;
			}
			default: break;
		}
	}
}
