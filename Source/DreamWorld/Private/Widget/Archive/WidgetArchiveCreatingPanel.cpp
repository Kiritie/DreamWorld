// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UWidgetArchiveCreatingPanel::UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ArchiveCreatingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetArchiveCreatingPanel::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetArchiveCreatingPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetArchiveCreatingPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

void UWidgetArchiveCreatingPanel::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
