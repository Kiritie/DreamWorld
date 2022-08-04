// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"
#include "Inventory/Slot/InventorySlot.h"

UWidgetInventoryEquipSlot::UWidgetInventoryEquipSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventoryEquipSlot::OnInitialize(UInventorySlot* InOwnerSlot)
{
	Super::OnInitialize(InOwnerSlot);
}
