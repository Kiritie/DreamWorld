// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"

#include "Common/DWCommonTypes.h"
#include "Gameplay/DWGameState.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

struct FDWGameSaveData;

ADWGameMode::ADWGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APawn::StaticClass();

	GameLevel = EDWGameLevel::Normal;
	bAutoJump = true;
}

void ADWGameMode::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWGameMode::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
	
	if(PHASEC(InPhase, EPhase::Lesser))
	{
		LoadSaveData(&USaveGameModuleBPLibrary::GetOrCreateSaveGame<UDWSettingSaveGame>(0)->GetSaveDataRef<FDWSettingSaveData>().GameData);
	}
}

void ADWGameMode::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWGameMode::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);

	if(PHASEC(InPhase, EPhase::Lesser))
	{
		USaveGameModuleBPLibrary::SaveSaveGame<UDWSettingSaveGame>(0, true);
	}
}

void ADWGameMode::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWGameSaveData>();

	SetGameLevel(SaveData.GameLevel);
	SetAutoJump(SaveData.bAutoJump);
}

void ADWGameMode::UnloadData(EPhase InPhase)
{
}

FSaveData* ADWGameMode::ToData()
{
	static FDWGameSaveData SaveData;
	SaveData = FDWGameSaveData();

	SaveData.GameLevel = GameLevel;
	SaveData.bAutoJump = bAutoJump;

	return &SaveData;
}
