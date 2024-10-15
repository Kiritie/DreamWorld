// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWPlayerSpawner.h"

#include "Ability/AbilityModuleTypes.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/Player/DWPlayerCharacterData.h"
#include "Common/CommonStatics.h"
#include "Gameplay/DWPlayerController.h"

ADWPlayerSpawner::ADWPlayerSpawner()
{
	InventoryInitType = EDWInventoryInitType::Default;
}

AActor* ADWPlayerSpawner::SpawnImpl_Implementation(const FAbilityItem& InAbilityItem)
{
	const auto& CharacterData = InAbilityItem.GetData<UDWPlayerCharacterData>();
	
	auto SaveData = FDWPlayerSaveData();
	SaveData.AssetID = CharacterData.GetPrimaryAssetId();
	SaveData.Name = *CharacterData.Name.ToString();
	SaveData.RaceID = CharacterData.RaceID;
	SaveData.Level = InAbilityItem.Level;
	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();
	SaveData.InventoryData = CharacterData.InventoryData;
	SaveData.InventoryInitType = InventoryInitType;

	ADWPlayerController* PlayerController = UCommonStatics::GetPlayerController<ADWPlayerController>();
	PlayerController->LoadSaveData(&SaveData, EPhase::PrimaryAndFinal);

	ADWPlayerCharacter* PlayerCharacter = PlayerController->GetPlayerPawn<ADWPlayerCharacter>();
	PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, true);

	return PlayerCharacter;
}

void ADWPlayerSpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
