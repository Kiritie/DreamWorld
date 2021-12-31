// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UWidgetArchiveCreatingPanel::UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("WorldCreatingPanel");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::UIOnly;
}

void UWidgetArchiveCreatingPanel::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(IsVisible())
	{
		ResetData();
	}
}
