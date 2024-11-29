// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/WidgetMainMenu.h"

UWidgetMainMenu::UWidgetMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MainMenu");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;

	SetIsFocusable(true);
}

void UWidgetMainMenu::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetMainMenu::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetMainMenu::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
