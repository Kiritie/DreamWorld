// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

UWidgetArchiveChoosingPanel::UWidgetArchiveChoosingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ArchiveChoosingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetArchiveChoosingPanel::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetArchiveChoosingPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetArchiveChoosingPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
