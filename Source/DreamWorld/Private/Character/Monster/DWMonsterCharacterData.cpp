#include "Character/Monster/DWMonsterCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWMonsterCharacterData::UDWMonsterCharacterData()
{
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(16);
	InventoryData.SplitInfos.Add(ESplitSlotType::Default, FSplitSlotInfo(0, 10));
	InventoryData.SplitInfos.Add(ESplitSlotType::Equip, FSplitSlotInfo(10, 6));

	AttackHitSound = nullptr;
	AttackMissSound = nullptr;
	AttackHitEffect = nullptr;
	AttackMissEffect = nullptr;
}
