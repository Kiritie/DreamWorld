// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/VitalityInventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Gameplay/DWGameInstance.h"
#include "Vitality/AbilityVitalityBase.h"

UAbilityVitalityInventory::UAbilityVitalityInventory()
{
	
}

void UAbilityVitalityInventory::Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos)
{
	Super::Initialize(InOwner, InSplitInfos);
}

void UAbilityVitalityInventory::LoadData(FInventorySaveData InInventoryData, AActor* InOwner)
{
	Super::LoadData(InInventoryData, InOwner);
}

FInventorySaveData UAbilityVitalityInventory::ToData(bool bSaved)
{
	return Super::ToData();
}

void UAbilityVitalityInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UAbilityVitalityInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

FQueryItemInfo UAbilityVitalityInventory::GetItemInfoByRange(EQueryItemType InActionType, FItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::GetItemInfoByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

AAbilityVitalityBase* UAbilityVitalityInventory::GetOwnerVitality() const
{
	return Cast<AAbilityVitalityBase>(OwnerActor);
}
