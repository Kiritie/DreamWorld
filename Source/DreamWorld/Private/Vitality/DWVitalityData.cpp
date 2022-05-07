#include "Vitality/DWVitalityData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWVitalityData::UDWVitalityData()
{
	InventoryData = FInventorySaveData();
	InventoryData.Items.SetNum(5);
	InventoryData.SplitInfos.Add(ESplitSlotType::Default, FSplitSlotInfo(0, 4));
}
