// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetCameraSettingPage.h"

#include "Widget/WidgetModuleBPLibrary.h"

UWidgetCameraSettingPage::UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CameraSettingPage");

	Title = FText::FromString(TEXT("摄像机"));
}

void UWidgetCameraSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetCameraSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetCameraSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetCameraSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();
}

void UWidgetCameraSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();
}

void UWidgetCameraSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetCameraSettingPage::CanApply_Implementation() const
{
	return false;
}

bool UWidgetCameraSettingPage::CanReset_Implementation() const
{
	return false;
}
