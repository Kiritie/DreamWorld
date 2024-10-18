// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWCharacterSpawner.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Actor/AbilityActorBase.h"
#include "Character/DWCharacterData.h"
#include "Common/DWCommonTypes.h"

ADWCharacterSpawner::ADWCharacterSpawner()
{

}

AActor* ADWCharacterSpawner::SpawnImpl_Implementation(const FAbilityItem& InAbilityItem)
{
	const auto& CharacterData = InAbilityItem.GetData<UDWCharacterData>();
	
	auto SaveData = FDWCharacterSaveData();
	SaveData.AssetID = CharacterData.GetPrimaryAssetId();
	SaveData.Name = *CharacterData.Name.ToString();
	SaveData.RaceID = CharacterData.RaceID;
	SaveData.Level = InAbilityItem.Level;
	SaveData.SpawnTransform = GetActorTransform();
	SaveData.InventoryData = CharacterData.InventoryData;

	return UAbilityModuleStatics::SpawnAbilityActor(&SaveData);
}

void ADWCharacterSpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
