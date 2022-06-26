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

void UVitalityInventory::Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos)
{
	Super::Initialize(InOwner, InSplitInfos);
}

void UVitalityInventory::LoadData(FInventorySaveData InInventoryData, AActor* InOwner)
{
	Super::LoadData(InInventoryData, InOwner);
}

FInventorySaveData UVitalityInventory::ToData()
{
	return Super::ToData();
}

void UVitalityInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UVitalityInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

FQueryItemInfo UVitalityInventory::GetItemInfoByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::GetItemInfoByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

ADWVitality* UVitalityInventory::GetOwnerVitality() const
{
	return Cast<ADWVitality>(OwnerActor);
}
