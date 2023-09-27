#include "Character/Human/DWHumanCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWHumanCharacterData::UDWHumanCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(22);
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(10);
	InventoryData.SplitItems.Add(ESlotSplitType::Shortcut).Items.SetNum(1);
	InventoryData.SplitItems.Add(ESlotSplitType::Auxiliary).Items.SetNum(1);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip).Items.SetNum(6);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill).Items.SetNum(4);
}
