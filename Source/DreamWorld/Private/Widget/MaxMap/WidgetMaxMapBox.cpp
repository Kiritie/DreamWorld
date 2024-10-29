// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MaxMap/WidgetMaxMapBox.h"

#include "Input/InputModuleStatics.h"
#include "Widget/WidgetModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"

UWidgetMaxMapBox::UWidgetMaxMapBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MaxMapBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	bWidgetAutoFocus = true;

	SetIsFocusable(true);
}

void UWidgetMaxMapBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetMaxMapBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetMaxMapBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
}

void UWidgetMaxMapBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}
