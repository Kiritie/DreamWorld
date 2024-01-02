// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Timer/WidgetTimerBox.h"

#include "Widget/WidgetModule.h"


UWidgetTimerBox::UWidgetTimerBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TimerBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_TimerBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetTimerBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTimerBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTimerBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetTimerBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
