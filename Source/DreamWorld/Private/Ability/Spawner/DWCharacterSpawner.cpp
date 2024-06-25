// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWCharacterSpawner.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Character/AbilityCharacterBase.h"
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
	SaveData.RaceID = NAME_None;
	SaveData.Level = InAbilityItem.Level;
	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();
	SaveData.InventoryData = CharacterData.InventoryData;

	AAbilityCharacterBase* Character = UAbilityModuleStatics::SpawnAbilityCharacter(&SaveData);

	return Character;
}

void ADWCharacterSpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
