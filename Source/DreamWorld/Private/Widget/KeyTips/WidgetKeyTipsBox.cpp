// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/KeyTips/WidgetKeyTipsBox.h"

#include "Widget/WidgetModule.h"


UWidgetKeyTipsBox::UWidgetKeyTipsBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("KeyTipsBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_KeyTipsBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetKeyTipsBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetKeyTipsBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetKeyTipsBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetKeyTipsBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
