// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetContextBox.h"

#include "Widget/WidgetModule.h"


UWidgetContextBox::UWidgetContextBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ContextBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::None;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetContextBox::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);
}

void UWidgetContextBox::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetContextBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetContextBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
