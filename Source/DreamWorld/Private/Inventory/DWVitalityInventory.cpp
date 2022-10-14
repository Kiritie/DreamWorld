// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/DWVitalityInventory.h"
#include "Inventory/Slot/DWInventoryEquipSlot.h"
#include "Inventory/Slot/DWInventorySkillSlot.h"

UDWVitalityInventory::UDWVitalityInventory()
{
	EquipSlotClass = UDWInventoryEquipSlot::StaticClass();
	SkillSlotClass = UDWInventorySkillSlot::StaticClass();
}

void UDWVitalityInventory::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* UDWVitalityInventory::ToData()
{
	return Super::ToData();
}

void UDWVitalityInventory::UnloadData(bool bForceMode)
{
	Super::UnloadData(bForceMode);
}

void UDWVitalityInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UDWVitalityInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

FQueryItemInfo UDWVitalityInventory::QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::QueryItemByRange(InActionType, InItem, InStartIndex, InEndIndex);
}
