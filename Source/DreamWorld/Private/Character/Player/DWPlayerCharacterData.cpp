#include "Character/Player/DWPlayerCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWPlayerCharacterData::UDWPlayerCharacterData()
{
	Nature = EDWCharacterNature::Player;
	
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(61);
	InventoryData.SplitInfos.Add(ESplitSlotType::Default, FSplitSlotInfo(0, 40));
	InventoryData.SplitInfos.Add(ESplitSlotType::Shortcut, FSplitSlotInfo(40, 10));
	InventoryData.SplitInfos.Add(ESplitSlotType::Auxiliary, FSplitSlotInfo(50, 1));
	InventoryData.SplitInfos.Add(ESplitSlotType::Equip, FSplitSlotInfo(51, 6));
	InventoryData.SplitInfos.Add(ESplitSlotType::Skill, FSplitSlotInfo(57, 4));
}
