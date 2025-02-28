// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Head/WidgetHeadBox.h"

#include "Widget/WidgetModule.h"

UWidgetHeadBox::UWidgetHeadBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("HeadBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_HeadBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetHeadBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetHeadBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);

	ClearTeamMates();
}

void UWidgetHeadBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetHeadBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetHeadBox::OnRefresh()
{
	Super::OnRefresh();
}
