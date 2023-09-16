// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

UWidgetArchiveChoosingPanel::UWidgetArchiveChoosingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ArchiveChoosingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetArchiveChoosingPanel::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetArchiveChoosingPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetArchiveChoosingPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
