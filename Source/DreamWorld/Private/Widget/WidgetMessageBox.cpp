// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetMessageBox.h"

#include "Widget/WidgetModule.h"


UWidgetMessageBox::UWidgetMessageBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MessageBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
}

void UWidgetMessageBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetMessageBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetMessageBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetMessageBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
