// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterModule.h"

#include "Asset/AssetModuleStatics.h"
#include "Character/Player/DWPlayerCharacterData.h"
#include "Common/CommonStatics.h"

IMPLEMENTATION_MODULE(UDWCharacterModule)

// ParamSets default values
UDWCharacterModule::UDWCharacterModule()
{
	PlayerBasicData = FDWPlayerBasicSaveData();
}

UDWCharacterModule::~UDWCharacterModule()
{
	TERMINATION_MODULE(UDWCharacterModule)
}

#if WITH_EDITOR
void UDWCharacterModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWCharacterModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWCharacterModule)
}
#endif

void UDWCharacterModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWCharacterModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWCharacterModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWCharacterModule::OnPause()
{
	Super::OnPause();
}

void UDWCharacterModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWCharacterModule::OnTermination(EPhase InPhase)
{
	Super::OnTermination(InPhase);
}

FDWPlayerBasicSaveData& UDWCharacterModule::GetPlayerBasicData()
{
	FPlayerRaceData PlayerRaceData;
	UAssetModuleStatics::GetDataTableRow(0, PlayerRaceData);
	PlayerBasicData.RaceID = PlayerRaceData.ID;
	if(PlayerRaceData.Items.IsValidIndex(0))
	{
		PlayerBasicData.AssetID = PlayerRaceData.Items[0].ID;
		PlayerBasicData.Level = PlayerRaceData.Items[0].Level;
	}
	PlayerBasicData.InitData();
	return PlayerBasicData;
}
