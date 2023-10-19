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

void UWidgetMessageBox::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetMessageBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetMessageBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetMessageBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
