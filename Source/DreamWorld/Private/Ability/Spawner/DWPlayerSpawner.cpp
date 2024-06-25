// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/Spawner/DWPlayerSpawner.h"

#include "Ability/AbilityModuleTypes.h"
#include "Character/Player/DWPlayerCharacterData.h"
#include "Common/CommonStatics.h"
#include "Common/DWCommonTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Playing.h"

ADWPlayerSpawner::ADWPlayerSpawner()
{

}

AActor* ADWPlayerSpawner::SpawnImpl_Implementation(const FAbilityItem& InAbilityItem)
{
	const auto& CharacterData = InAbilityItem.GetData<UDWPlayerCharacterData>();
	
	auto SaveData = FDWPlayerSaveData();
	SaveData.AssetID = CharacterData.GetPrimaryAssetId();
	SaveData.Name = *CharacterData.Name.ToString();
	SaveData.RaceID = NAME_None;
	SaveData.Level = InAbilityItem.Level;
	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();
	SaveData.InventoryInitType = EDWInventoryInitType::All;

	UCommonStatics::GetPlayerController<ADWPlayerController>()->LoadSaveData(&SaveData, EPhase::PrimaryAndFinal);

	UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Playing>();

	return UCommonStatics::GetPlayerPawn();
}

void ADWPlayerSpawner::DestroyImpl_Implementation(AActor* InAbilityActor)
{
	Super::DestroyImpl_Implementation(InAbilityActor);
}
