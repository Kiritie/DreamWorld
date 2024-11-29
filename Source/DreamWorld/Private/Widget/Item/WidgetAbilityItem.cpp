// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Item/WidgetAbilityItem.h"

#include "Widget/WidgetModuleStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widget/Item/Info/WidgetAbilityItemInfoBox.h"

UWidgetAbilityItem::UWidgetAbilityItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ImgIcon = nullptr;
	TxtName = nullptr;
}

void UWidgetAbilityItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetAbilityItem::OnRefresh()
{
	Super::OnRefresh();
	
	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();
		ImgIcon->SetBrushResourceObject(ItemData.Icon);
		TxtName->SetText(ItemData.Name);
	}
}

void UWidgetAbilityItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	TArray<FAbilityItem> Items = { Item };
	UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ &Items });
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
