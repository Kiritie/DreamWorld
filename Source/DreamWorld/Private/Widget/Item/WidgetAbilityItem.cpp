// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Item/WidgetAbilityItem.h"

#include "Widget/WidgetModuleStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Widget/Item/WidgetAbilityItemInfoBox.h"

class UWidgetAbilityItemInfoBox;

UWidgetAbilityItem::UWidgetAbilityItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetAbilityItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const auto& ItemData = Item.GetData();
	UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ ItemData.Name, ItemData.Detail });
}

void UWidgetAbilityItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UWidgetModuleStatics::CloseUserWidget<UWidgetAbilityItemInfoBox>();
}

FReply UWidgetAbilityItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	OnSelected();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

FReply UWidgetAbilityItem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(const auto ItemInfoBoxSlot = Cast<UCanvasPanelSlot>(UWidgetModuleStatics::GetUserWidget<UWidgetAbilityItemInfoBox>()->Slot))
	{
		float PosX, PosY;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UCommonStatics::GetPlayerController(), PosX, PosY);
		ItemInfoBoxSlot->SetPosition(FVector2D(PosX, PosY));
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetAbilityItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWidgetAbilityItem::OnSelected()
{
}

void UWidgetAbilityItem::OnUnSelected()
{
}
