// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MiniMap/WidgetMiniMapBox.h"

#include "Widget/WidgetModule.h"


UWidgetMiniMapBox::UWidgetMiniMapBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MiniMapBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_MiniMapBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetMiniMapBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetMiniMapBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetMiniMapBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetMiniMapBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
