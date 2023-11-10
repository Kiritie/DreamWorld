// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"

#include "Common/DWCommonTypes.h"
#include "Gameplay/DWGameState.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

ADWGameMode::ADWGameMode()
{
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
		USaveGameModuleStatics::LoadOrCreateSaveGame<UDWSettingSaveGame>(0);
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
		USaveGameModuleStatics::SaveSaveGame<UDWSettingSaveGame>(0, true);
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
