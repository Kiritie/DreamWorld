#include "Character/NPC/DWNPCCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWNPCCharacterData::UDWNPCCharacterData()
{
	Nature = EDWCharacterNature::NPC;
	Craft = EDWCharacterCraft::None;
	
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(40);
	InventoryData.SplitItems.Add(ESlotSplitType::Shortcut).Items.SetNum(10);
	InventoryData.SplitItems.Add(ESlotSplitType::Auxiliary).Items.SetNum(1);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip).Items.SetNum(6);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill).Items.SetNum(4);
}
