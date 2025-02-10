// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"

#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"

UWidgetInventoryEquipSlot::UWidgetInventoryEquipSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventoryEquipSlot::OnRefresh()
{
	Super::OnRefresh();

	SetVisibility(OwnerSlot->IsEnabled() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
