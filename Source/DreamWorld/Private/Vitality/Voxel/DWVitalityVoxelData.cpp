#include "Vitality/Voxel/DWVitalityVoxelData.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Vitality/Voxel/DWVitalityVoxel.h"

UDWVitalityVoxelData::UDWVitalityVoxelData()
{
	MaxLevel = 10;

	VoxelID = FPrimaryAssetId();

	Class = ADWVitalityVoxel::StaticClass();
}
