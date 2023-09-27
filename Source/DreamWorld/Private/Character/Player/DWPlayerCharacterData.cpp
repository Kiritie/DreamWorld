#include "Character/Player/DWPlayerCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWPlayerCharacterData::UDWPlayerCharacterData()
{
	Nature = EDWCharacterNature::Player;
	
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(61);
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(40);
	InventoryData.SplitItems.Add(ESlotSplitType::Shortcut).Items.SetNum(10);
	InventoryData.SplitItems.Add(ESlotSplitType::Auxiliary).Items.SetNum(1);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip).Items.SetNum(6);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill).Items.SetNum(4);
}
