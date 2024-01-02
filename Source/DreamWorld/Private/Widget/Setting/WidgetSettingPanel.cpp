// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/WidgetSettingPanel.h"

#include "Widget/WidgetModuleStatics.h"

UWidgetSettingPanel::UWidgetSettingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetSettingPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetSettingPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetSettingPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

FReply UWidgetSettingPanel::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == FKey("Escape"))
	{
		Close();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
