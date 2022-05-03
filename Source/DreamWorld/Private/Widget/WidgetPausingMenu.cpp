// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetPausingMenu.h"

UWidgetPausingMenu::UWidgetPausingMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("PausingMenu");
	WidgetCategory = EWidgetCategory::Temporary;
	InputMode = EInputMode::GameAndUI;
}

void UWidgetPausingMenu::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetPausingMenu::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetPausingMenu::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}
