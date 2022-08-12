// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetGameHUD.h"

#include "Ability/Components/InteractionComponent.h"
#include "Character/DWCharacter.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Input/EventHandle_ChangeInputMode.h"
#include "Main/MainModule.h"
#include "Widget/WidgetModule.h"


UWidgetGameHUD::UWidgetGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameHUD");
	WidgetCategory = EWidgetCategory::Permanent;
	InputMode = EInputMode::GameOnly;
}

void UWidgetGameHUD::OnCreate_Implementation(AActor* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	UEventModuleBPLibrary::SubscribeEvent(UEventHandle_ChangeInputMode::StaticClass(), this, FName("OnChangeInputMode"));
}

void UWidgetGameHUD::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetGameHUD::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetGameHUD::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}

void UWidgetGameHUD::RefreshActions()
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwnerActor()))
	{
		if(const IInteractionAgentInterface* OverlappingAgent = OwnerCharacter->GetInteractionComponent()->GetOverlappingAgent())
		{
			ShowActions(OverlappingAgent->GetInteractionComponent()->GetValidInteractActions(OwnerCharacter));
		}
		else
		{
			HideActions();
		}
	}
}

void UWidgetGameHUD::OnChangeInputMode(UObject* InSender, UEventHandle_ChangeInputMode* InEventHandle)
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(OwnerActor))
	{
		if(!OwnerCharacter->IsDead() && InEventHandle->InputMode == EInputMode::GameOnly)
		{
			SetCrosshairVisible(true);
		}
		else
		{
			SetCrosshairVisible(true);
		}
	}
	else
	{
		SetCrosshairVisible(true);
	}
}
