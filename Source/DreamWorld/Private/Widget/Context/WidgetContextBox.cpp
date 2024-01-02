// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Context/WidgetContextBox.h"

#include "Widget/WidgetModule.h"


UWidgetContextBox::UWidgetContextBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ContextBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_ContextBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetContextBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetContextBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetContextBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetContextBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
