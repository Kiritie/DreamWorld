#include "Character/Monster/DWMonsterCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWMonsterCharacterData::UDWMonsterCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default, 10);
	InventoryData.SplitItems.Add(ESlotSplitType::Skill, 4);
}
