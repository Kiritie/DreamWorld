// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Interaction/WidgetInteractionBox.h"

#include "Widget/WidgetModule.h"


UWidgetInteractionBox::UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InteractionBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_InteractionBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
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

void UWidgetInteractionBox::OnRefresh()
{
	Super::OnRefresh();
}
