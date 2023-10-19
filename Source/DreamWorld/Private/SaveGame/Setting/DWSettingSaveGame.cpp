// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Setting/DWSettingSaveGame.h"

#include "Audio/AudioModule.h"
#include "Camera/CameraModule.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/DWGameMode.h"

UDWSettingSaveGame::UDWSettingSaveGame()
{
	SaveName = FName("Setting");
}

void UDWSettingSaveGame::OnCreate_Implementation(int32 InIndex)
{
	Super::OnCreate_Implementation(InIndex);

	Refresh();
}

void UDWSettingSaveGame::OnLoad_Implementation(EPhase InPhase)
{
	Super::OnLoad_Implementation(InPhase);

	UCommonBPLibrary::GetGameMode<ADWGameMode>()->LoadSaveData(&SaveData.GameData, InPhase);
	AAudioModule::Get()->LoadSaveData(&SaveData.AudioData, InPhase);
	ACameraModule::Get()->LoadSaveData(&SaveData.CameraData, InPhase);
}

void UDWSettingSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);
}

void UDWSettingSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	SaveData.GameData = UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetSaveDataRef<FDWGameSaveData>(true);
	SaveData.AudioData = AAudioModule::Get()->GetSaveDataRef<FDWAudioModuleSaveData>(true);
	SaveData.CameraData = ACameraModule::Get()->GetSaveDataRef<FDWCameraModuleSaveData>(true);
}
