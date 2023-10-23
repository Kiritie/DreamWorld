// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/WidgetMainMenu.h"

UWidgetMainMenu::UWidgetMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MainMenu");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetMainMenu::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetMainMenu::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetMainMenu::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
