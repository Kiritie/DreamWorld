// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/DWVitalityInventory.h"
#include "Inventory/Slot/DWInventoryEquipSlot.h"
#include "Inventory/Slot/DWInventorySkillSlot.h"

UDWVitalityInventory::UDWVitalityInventory()
{
	EquipSlotClass = UDWInventoryEquipSlot::StaticClass();
	SkillSlotClass = UDWInventorySkillSlot::StaticClass();
}

void UDWVitalityInventory::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWVitalityInventory::ToData()
{
	return Super::ToData();
}

void UDWVitalityInventory::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}
