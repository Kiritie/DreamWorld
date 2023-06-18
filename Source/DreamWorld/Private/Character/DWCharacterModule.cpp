// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterModule.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Ability/AbilityModuleTypes.h"
#include "Character/Player/DWPlayerCharacterData.h"

IMPLEMENTATION_MODULE(ADWCharacterModule)

// ParamSets default values
ADWCharacterModule::ADWCharacterModule()
{
	PlayerBasicData = FDWPlayerBasicSaveData();
}

ADWCharacterModule::~ADWCharacterModule()
{
	TERMINATION_MODULE(ADWCharacterModule)
}

#if WITH_EDITOR
void ADWCharacterModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWCharacterModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWCharacterModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWCharacterModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWCharacterModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWCharacterModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWCharacterModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWCharacterModule::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

FDWPlayerBasicSaveData& ADWCharacterModule::GetPlayerBasicData()
{
	FPlayerRaceData PlayerRaceData;
	UAssetModuleBPLibrary::GetDataTableRow(0, PlayerRaceData);
	PlayerBasicData.RaceID = PlayerRaceData.ID;
	if(PlayerRaceData.Items.Num() > 0)
	{
		PlayerBasicData.ID = PlayerRaceData.Items[0].ID;
		PlayerBasicData.Level = PlayerRaceData.Items[0].Level;
	}
	PlayerBasicData.InventoryData = UAssetModuleBPLibrary::LoadPrimaryAssetRef<UDWPlayerCharacterData>(PlayerBasicData.ID).InventoryData;
	return PlayerBasicData;
}
