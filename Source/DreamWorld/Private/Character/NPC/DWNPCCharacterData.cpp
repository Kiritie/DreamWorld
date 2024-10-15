#include "Character/NPC/DWNPCCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWNPCCharacterData::UDWNPCCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(40);
}
