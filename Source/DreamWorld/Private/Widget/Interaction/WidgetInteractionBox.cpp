// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Interaction/WidgetInteractionBox.h"

#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Widget/WidgetModule.h"
#include "Widget/Item/WidgetAbilityPreviewItem.h"

UWidgetInteractionBox::UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InteractionBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_InteractionBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
	
	WBP_PreviewItem = nullptr;
}

void UWidgetInteractionBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetInteractionBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetInteractionBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetInteractionBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetInteractionBox::ShowInteractActions_Implementation(const TScriptInterface<IInteractionAgentInterface>& InInteractionAgent, const TArray<EInteractAction>& InActions)
{
	if(InActions.IsEmpty()) return;
	
	if(AAbilityPickUpBase* PickUp = Cast<AAbilityPickUpBase>(InInteractionAgent.GetObject()))
	{
		WBP_PreviewItem->Init({ &PickUp->GetItem() });
		WBP_PreviewItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetInteractionBox::HideInteractActions_Implementation()
{
	WBP_PreviewItem->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetInteractionBox::OnRefresh()
{
	Super::OnRefresh();
}
