// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/WidgetSettingPanel.h"

#include "Widget/WidgetModuleStatics.h"

UWidgetSettingPanel::UWidgetSettingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ChildNames.Empty();
	ChildNames.Add(FName("GameSettingPage"));
	ChildNames.Add(FName("AudioSettingPage"));
	ChildNames.Add(FName("VideoSettingPage"));
	ChildNames.Add(FName("CameraSettingPage"));
	ChildNames.Add(FName("InputSettingPage"));
}

void UWidgetSettingPanel::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
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
