#include "Character/Player/DWPlayerCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWPlayerCharacterData::UDWPlayerCharacterData()
{
	Nature = EDWCharacterNature::Player;
	
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default, 40);
	InventoryData.SplitItems.Add(ESlotSplitType::Shortcut, 10);
	InventoryData.SplitItems.Add(ESlotSplitType::Auxiliary, 1);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip, 10);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill, 4);
}
