#include "Character/NPC/DWNPCCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWNPCCharacterData::UDWNPCCharacterData()
{
	Nature = EDWCharacterNature::NPC;
	Craft = EDWCharacterCraft::None;
	
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(100);
}
