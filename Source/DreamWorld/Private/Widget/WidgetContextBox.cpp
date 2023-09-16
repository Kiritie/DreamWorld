// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetContextBox.h"

#include "Character/DWCharacter.h"
#include "Main/MainModule.h"
#include "Widget/WidgetModule.h"


UWidgetContextBox::UWidgetContextBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ContextBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::None;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetContextBox::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetContextBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetContextBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetContextBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
