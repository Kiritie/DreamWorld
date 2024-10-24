// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWVitalitySpawner.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Vitality/DWVitalityData.h"
#include "Common/DWCommonTypes.h"

ADWVitalitySpawner::ADWVitalitySpawner()
{

}

AActor* ADWVitalitySpawner::SpawnImpl_Implementation(const FAbilityItem& InAbilityItem)
{
	const auto& VitalityData = InAbilityItem.GetData<UDWVitalityData>();
	
	auto SaveData = FDWVitalitySaveData();
	SaveData.AssetID = VitalityData.GetPrimaryAssetId();
	SaveData.Name = *VitalityData.Name.ToString();
	SaveData.RaceID = VitalityData.RaceID;
	SaveData.Level = InAbilityItem.Level;
	SaveData.SpawnTransform = GetActorTransform();
	SaveData.InventoryData = VitalityData.InventoryData;
	SaveData.InventoryData.FillItems();

	return UAbilityModuleStatics::SpawnAbilityActor(&SaveData);
}

void ADWVitalitySpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
