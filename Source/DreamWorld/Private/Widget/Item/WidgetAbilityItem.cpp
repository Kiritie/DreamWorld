// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Item/WidgetAbilityItem.h"

#include "Widget/WidgetModuleStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Widget/Item/Info/WidgetAbilityItemInfoBox.h"

UWidgetAbilityItem::UWidgetAbilityItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetAbilityItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ &Item });
}

void UWidgetAbilityItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UWidgetModuleStatics::CloseUserWidget<UWidgetAbilityItemInfoBox>();
}

FReply UWidgetAbilityItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

FReply UWidgetAbilityItem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(const UWidgetAbilityItemInfoBox* ItemInfoBox = UWidgetModuleStatics::GetUserWidget<UWidgetAbilityItemInfoBox>())
	{
		if(const auto ItemInfoBoxSlot = Cast<UCanvasPanelSlot>(ItemInfoBox->Slot))
		{
			ItemInfoBoxSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
		}
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetAbilityItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
