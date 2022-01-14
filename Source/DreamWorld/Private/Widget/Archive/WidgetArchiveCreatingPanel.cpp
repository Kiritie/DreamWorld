// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UWidgetArchiveCreatingPanel::UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("WorldCreatingPanel");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::UIOnly;
}

void UWidgetArchiveCreatingPanel::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetArchiveCreatingPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetArchiveCreatingPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}

void UWidgetArchiveCreatingPanel::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
