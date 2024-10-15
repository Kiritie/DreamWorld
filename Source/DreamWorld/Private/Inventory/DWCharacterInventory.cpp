// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/DWCharacterInventory.h"
#include "Inventory/Slot/DWInventoryEquipSlot.h"
#include "Inventory/Slot/DWInventorySkillSlot.h"
#include "Ability/Inventory/Slot/AbilityInventoryEquipSlotBase.h"
#include "Item/Equip/DWEquipData.h"

UDWCharacterInventory::UDWCharacterInventory()
{
	EquipSlotClass = UDWInventoryEquipSlot::StaticClass();
	SkillSlotClass = UDWInventorySkillSlot::StaticClass();
}

void UDWCharacterInventory::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWCharacterInventory::ToData()
{
	return Super::ToData();
}

void UDWCharacterInventory::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

UAbilityInventoryEquipSlotBase* UDWCharacterInventory::GetEquipSlotByPartType(EDWEquipPartType InPartType)
{
	auto EquipSlots = GetSlotsBySplitType(ESlotSplitType::Equip);
	for(int32 i = 0; i < EquipSlots.Num(); i++)
	{
		if(EquipSlots[i]->GetItem().GetData<UDWEquipData>().PartType == InPartType)
		{
			return Cast<UAbilityInventoryEquipSlotBase>(EquipSlots[i]);
		}
	}
	return nullptr;
}
