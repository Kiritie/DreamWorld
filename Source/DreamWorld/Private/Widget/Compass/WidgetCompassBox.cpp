// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Compass/WidgetCompassBox.h"

#include "Widget/WidgetModule.h"


UWidgetCompassBox::UWidgetCompassBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CompassBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_CompassBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetCompassBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetCompassBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetCompassBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetCompassBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
