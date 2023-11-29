// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Loading/WidgetLoadingPanel.h"

UWidgetLoadingPanel::UWidgetLoadingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("LoadingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetZOrder = 15;
}

void UWidgetLoadingPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetLoadingPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetLoadingPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
