// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

UWidgetArchiveChoosingPanel::UWidgetArchiveChoosingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("RoleChoosingPanel");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::UIOnly;
}

void UWidgetArchiveChoosingPanel::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetArchiveChoosingPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetArchiveChoosingPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}
