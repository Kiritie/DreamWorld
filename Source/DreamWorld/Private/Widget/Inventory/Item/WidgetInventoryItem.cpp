// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Item/WidgetInventoryItem.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/Item/WidgetInventoryItemInfoBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonBPLibrary.h"

UWidgetInventoryItem::UWidgetInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const auto& ItemData = Item.GetData();
	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ ItemData.Name, ItemData.Detail });
}

void UWidgetInventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryItemInfoBox>();
}

FReply UWidgetInventoryItem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(const auto ItemInfoBoxSlot = Cast<UCanvasPanelSlot>(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryItemInfoBox>()->Slot))
	{
		float PosX, PosY;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UCommonBPLibrary::GetPlayerController(), PosX, PosY);
		ItemInfoBoxSlot->SetPosition(FVector2D(PosX, PosY));
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetInventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
