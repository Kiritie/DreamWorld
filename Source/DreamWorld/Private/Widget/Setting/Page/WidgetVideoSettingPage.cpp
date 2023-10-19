// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetVideoSettingPage.h"

#include "Widget/WidgetModuleBPLibrary.h"

UWidgetVideoSettingPage::UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VideoSettingPage");

	Title = FText::FromString(TEXT("视频"));
}

void UWidgetVideoSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetVideoSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetVideoSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetVideoSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();
}

void UWidgetVideoSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();
}

void UWidgetVideoSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetVideoSettingPage::CanApply_Implementation() const
{
	return false;
}

bool UWidgetVideoSettingPage::CanReset_Implementation() const
{
	return false;
}
