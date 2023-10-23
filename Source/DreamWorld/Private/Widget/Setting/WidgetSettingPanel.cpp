// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/WidgetSettingPanel.h"

#include "Components/TextBlock.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Page/WidgetSettingPageBase.h"

UWidgetSettingPanel::UWidgetSettingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("SettingPanel");

	ChildNames.Add(FName("GameSettingPage"));
	ChildNames.Add(FName("AudioSettingPage"));
	ChildNames.Add(FName("VideoSettingPage"));
	ChildNames.Add(FName("CameraSettingPage"));

	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;

	WidgetZOrder = 10;

	SetIsFocusable(true);
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

void UWidgetSettingPanel::ChangePage(int32 InPageIndex)
{
	PageIndex = InPageIndex;
	if(const auto Page = GetChild<UWidgetSettingPageBase>(PageIndex))
	{
		Page->Open();
		Txt_Title->SetText(Page->Title);
	}
}
