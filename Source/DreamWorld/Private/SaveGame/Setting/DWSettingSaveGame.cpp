// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Setting/DWSettingSaveGame.h"

#include "Audio/AudioModule.h"
#include "Audio/DWAudioModule.h"
#include "Camera/DWCameraModule.h"
#include "Common/CommonStatics.h"
#include "Gameplay/DWGameMode.h"
#include "Input/DWInputModule.h"
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

	UCommonStatics::GetGameMode<ADWGameMode>()->LoadSaveData(&SaveData.GameData, InPhase);
	UDWAudioModule::Get().LoadSaveData(&SaveData.AudioData, InPhase);
	UDWVideoModule::Get().LoadSaveData(&SaveData.VideoData, InPhase);
	UDWCameraModule::Get().LoadSaveData(&SaveData.CameraData, InPhase);
	UDWInputModule::Get().LoadSaveData(&SaveData.InputData, InPhase);
}

void UDWSettingSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);
}

void UDWSettingSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	SaveData.GameData = UCommonStatics::GetGameMode<ADWGameMode>()->GetSaveDataRef<FDWGameSaveData>(true);
	SaveData.AudioData = UDWAudioModule::Get().GetSaveDataRef<FDWAudioModuleSaveData>(true);
	SaveData.VideoData = UDWVideoModule::Get().GetSaveDataRef<FDWVideoModuleSaveData>(true);
	SaveData.CameraData = UDWCameraModule::Get().GetSaveDataRef<FDWCameraModuleSaveData>(true);
	SaveData.InputData = UDWInputModule::Get().GetSaveDataRef<FDWInputModuleSaveData>(true);
}
