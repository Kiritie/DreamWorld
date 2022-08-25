#include "Vitality/Voxel/DWVitalityVoxelData.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Vitality/Voxel/DWVitalityVoxel.h"

UDWVitalityVoxelData::UDWVitalityVoxelData()
{
	VoxelID = FPrimaryAssetId();

	Class = ADWVitalityVoxel::StaticClass();
}
