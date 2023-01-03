// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterModule.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Ability/AbilityModuleTypes.h"
#include "Character/Player/DWPlayerCharacterData.h"

MODULE_INSTANCE_IMPLEMENTATION(ADWCharacterModule)

// ParamSets default values
ADWCharacterModule::ADWCharacterModule()
{
	PlayerBasicData = FDWPlayerBasicSaveData();
}

#if WITH_EDITOR
void ADWCharacterModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWCharacterModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWCharacterModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWCharacterModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
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
