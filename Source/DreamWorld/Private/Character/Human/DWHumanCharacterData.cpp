#include "Character/Human/DWHumanCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWHumanCharacterData::UDWHumanCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(22);
	InventoryData.SplitInfos.Add(ESplitSlotType::Default, FSplitSlotInfo(0, 10));
	InventoryData.SplitInfos.Add(ESplitSlotType::Shortcut, FSplitSlotInfo(10, 1));
	InventoryData.SplitInfos.Add(ESplitSlotType::Auxiliary, FSplitSlotInfo(11, 1));
	InventoryData.SplitInfos.Add(ESplitSlotType::Equip, FSplitSlotInfo(12, 6));
	InventoryData.SplitInfos.Add(ESplitSlotType::Skill, FSplitSlotInfo(18, 4));
}
