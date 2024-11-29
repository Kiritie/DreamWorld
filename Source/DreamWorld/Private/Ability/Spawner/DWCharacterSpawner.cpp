// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWCharacterSpawner.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/AbilityModuleTypes.h"
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
	SaveData.InitInventoryData();
	if(CharacterData.Dialogues.Num() > 0)
	{
		SaveData.Dialogue = CharacterData.Dialogues[FMath::RandRange(0, CharacterData.Dialogues.Num() - 1)];
	}

	return UAbilityModuleStatics::SpawnAbilityActor(&SaveData);
}

void ADWCharacterSpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
