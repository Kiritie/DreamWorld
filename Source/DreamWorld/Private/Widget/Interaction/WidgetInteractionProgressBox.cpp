// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Interaction/WidgetInteractionProgressBox.h"

#include "Widget/WidgetModule.h"
#include "Widget/Common/CommonTextBlockN.h"
#include "Widget/Item/WidgetAbilityItem.h"

UWidgetInteractionProgressBox::UWidgetInteractionProgressBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InteractionProgressBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_InteractionProgressBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
}

void UWidgetInteractionProgressBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetInteractionProgressBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetInteractionProgressBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	if(InParams.IsValidIndex(0))
	{
		TxtTargetName->SetText(InParams[0]);
	}
	if(InParams.IsValidIndex(1))
	{
		TxtActionName->SetText(InParams[1]);
	}

	SetProgress(0.f);
}

void UWidgetInteractionProgressBox::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetInteractionProgressBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
