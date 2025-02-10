#include "Character/Human/DWHumanCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWHumanCharacterData::UDWHumanCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default, 10);
	InventoryData.SplitItems.Add(ESlotSplitType::Shortcut, 1);
	InventoryData.SplitItems.Add(ESlotSplitType::Auxiliary, 1);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip, 10);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill, 4);
}
