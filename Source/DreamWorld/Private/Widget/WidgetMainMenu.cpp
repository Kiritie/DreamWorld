// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetMainMenu.h"

#include "Main/MainModule.h"

UWidgetMainMenu::UWidgetMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MainMenu");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetMainMenu::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetMainMenu::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetMainMenu::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
