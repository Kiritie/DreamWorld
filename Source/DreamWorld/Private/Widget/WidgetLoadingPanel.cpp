// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetLoadingPanel.h"

UWidgetLoadingPanel::UWidgetLoadingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("LoadingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetZOrder = 5;
}

void UWidgetLoadingPanel::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetLoadingPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetLoadingPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
