// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/Preview/WidgetInventoryPreviewSlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryItemInfoBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Global/GlobalBPLibrary.h"

UWidgetInventoryPreviewSlot::UWidgetInventoryPreviewSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Item = FAbilityItem::Empty;
}

void UWidgetInventoryPreviewSlot::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{

}

void UWidgetInventoryPreviewSlot::OnDespawn_Implementation(bool bRecovery)
{
	Owner = nullptr;
	Item = FAbilityItem::Empty;
	RemoveFromParent();
}

void UWidgetInventoryPreviewSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const auto& ItemData = Item.GetData();
	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ FParameter::MakeText(ItemData.Name), FParameter::MakeText(ItemData.Detail) });
}

void UWidgetInventoryPreviewSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryItemInfoBox>();
}

FReply UWidgetInventoryPreviewSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(auto InventoryItemInfoBox = UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryItemInfoBox>())
	{
		if(auto InventoryItemInfoBoxSlot = Cast<UCanvasPanelSlot>(InventoryItemInfoBox->Slot))
		{
			float PosX, PosY;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGlobalBPLibrary::GetPlayerController(), PosX, PosY);
			InventoryItemInfoBoxSlot->SetPosition(FVector2D(PosX, PosY));
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetInventoryPreviewSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWidgetInventoryPreviewSlot::OnInitialize_Implementation(UUserWidgetBase* InOwner, FAbilityItem InItem)
{
	Owner = InOwner;
	Item = InItem;
	OnRefresh();
}

void UWidgetInventoryPreviewSlot::OnRefresh_Implementation()
{

}
