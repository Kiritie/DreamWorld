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
	ChildNames =
	{
		FName("InventoryBar"),
	    FName("InventoryPanel"),
	    FName("GeneratePanel"),
	    FName("ContextBox"),
	    FName("ItemInfoBox"),
	    FName("TaskInfoBox")
	};
	WidgetType = EWidgetType::Permanent;
	InputMode = EInputMode::GameOnly;

	WidgetRefreshType = EWidgetRefreshType::Tick;

	bIsFocusable = true;
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

void UWidgetGameHUD::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(OwnerActor))
	{
		if(auto Interaction = OwnerCharacter->GetInteractionComponent()->GetInteractingComponent())
		{
			ShowActions(Interaction->GetValidInteractActions(OwnerCharacter));
		}
		else
		{
			HideActions();
		}
	}
}

FReply UWidgetGameHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == FKey("Escape"))
	{
		if(TemporaryChild)
		{
			TemporaryChild->Close();
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
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
			SetCrosshairVisible(false);
		}
	}
	else
	{
		SetCrosshairVisible(true);
	}
}
