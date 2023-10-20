// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Setting/DWSettingSaveGame.h"

#include "Audio/AudioModule.h"
#include "Audio/DWAudioModule.h"
#include "Camera/DWCameraModule.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "Video/DWVideoModule.h"

UDWSettingSaveGame::UDWSettingSaveGame()
{
	SaveName = FName("Setting");
}

void UDWSettingSaveGame::OnCreate_Implementation(int32 InIndex)
{
	Super::OnCreate_Implementation(InIndex);

	Refresh();
	SetDefaultData(GetSaveData());
}

void UDWSettingSaveGame::OnLoad_Implementation(EPhase InPhase)
{
	Super::OnLoad_Implementation(InPhase);

	UCommonBPLibrary::GetGameMode<ADWGameMode>()->LoadSaveData(&SaveData.GameData, InPhase);
	ADWAudioModule::Get()->LoadSaveData(&SaveData.AudioData, InPhase);
	ADWVideoModule::Get()->LoadSaveData(&SaveData.VideoData, InPhase);
	ADWCameraModule::Get()->LoadSaveData(&SaveData.CameraData, InPhase);
}

void UDWSettingSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);
}

void UDWSettingSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	SaveData.GameData = UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetSaveDataRef<FDWGameSaveData>(true);
	SaveData.AudioData = ADWAudioModule::Get()->GetSaveDataRef<FDWAudioModuleSaveData>(true);
	SaveData.VideoData = ADWVideoModule::Get()->GetSaveDataRef<FDWVideoModuleSaveData>(true);
	SaveData.CameraData = ADWCameraModule::Get()->GetSaveDataRef<FDWCameraModuleSaveData>(true);
}
