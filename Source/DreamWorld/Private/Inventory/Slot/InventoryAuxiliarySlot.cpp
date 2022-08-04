// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventoryAuxiliarySlot.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"

UInventoryAuxiliarySlot::UInventoryAuxiliarySlot()
{
}

void UInventoryAuxiliarySlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType);
}
