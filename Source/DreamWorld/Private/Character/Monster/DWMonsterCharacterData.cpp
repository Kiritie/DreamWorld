#include "Character/Monster/DWMonsterCharacterData.h"

#include "Ability/AbilityModuleStatics.h"
#include "Inventory/DWCharacterInventory.h"

UDWMonsterCharacterData::UDWMonsterCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.InventoryClass = UDWCharacterInventory::StaticClass();
	InventoryData.SplitItems.Add(ESlotSplitType::Default).Items.SetNum(10);
	InventoryData.SplitItems.Add(ESlotSplitType::Equip).Items.SetNum(6);

	AttackHitSound = nullptr;
	AttackMissSound = nullptr;
	AttackHitEffect = nullptr;
	AttackMissEffect = nullptr;
}
