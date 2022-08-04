// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/VitalityInventory.h"

#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Gameplay/DWGameInstance.h"
#include "Vitality/DWVitality.h"

UVitalityInventory::UVitalityInventory()
{
	
}

void UVitalityInventory::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* UVitalityInventory::ToData()
{
	return Super::ToData();
}

void UVitalityInventory::UnloadData(bool bForceMode)
{
	Super::UnloadData(bForceMode);
}

void UVitalityInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UVitalityInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

FQueryItemInfo UVitalityInventory::QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::QueryItemByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

ADWVitality* UVitalityInventory::GetOwnerVitality() const
{
	return Cast<ADWVitality>(OwnerActor);
}
