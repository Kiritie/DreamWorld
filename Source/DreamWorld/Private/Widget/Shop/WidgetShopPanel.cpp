// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Shop/WidgetShopPanel.h"

UWidgetShopPanel::UWidgetShopPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ShopPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	WidgetZOrder = 1;

	SetIsFocusable(true);
}
