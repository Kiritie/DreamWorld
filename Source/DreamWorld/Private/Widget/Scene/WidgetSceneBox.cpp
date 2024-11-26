// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Scene/WidgetSceneBox.h"

#include "Widget/WidgetModule.h"


UWidgetSceneBox::UWidgetSceneBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("SceneBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_SceneBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
	WidgetRefreshType = EWidgetRefreshType::Tick;
}

void UWidgetSceneBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetSceneBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetSceneBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetSceneBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
