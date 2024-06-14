// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/DWSettingModule.h"

#include "Audio/AudioModule.h"
#include "Audio/DWAudioModule.h"
#include "Camera/DWCameraModule.h"
#include "Common/CommonStatics.h"
#include "Common/DWCommonTypes.h"
#include "Input/DWInputModule.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Video/DWVideoModule.h"

IMPLEMENTATION_MODULE(UDWSettingModule)

// ParamSets default values
UDWSettingModule::UDWSettingModule()
{
	bModuleAutoSave = true;
	ModuleSaveGame = UDWSettingSaveGame::StaticClass();

	GameLevel = EDWGameLevel::Normal;
	bAutoJump = true;
}

UDWSettingModule::~UDWSettingModule()
{
	TERMINATION_MODULE(UDWSettingModule)
}

#if WITH_EDITOR
void UDWSettingModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWSettingModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWSettingModule)
}
#endif

void UDWSettingModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWSettingModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWSettingModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWSettingModule::OnPause()
{
	Super::OnPause();
}

void UDWSettingModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWSettingModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWSettingModuleSaveData>();

	SetGameLevel(SaveData.GameData.GameLevel);
	SetAutoJump(SaveData.GameData.bAutoJump);
	
	UDWAudioModule::Get().LoadSaveData(&SaveData.AudioData, InPhase);
	UDWVideoModule::Get().LoadSaveData(&SaveData.VideoData, InPhase);
	UDWCameraModule::Get().LoadSaveData(&SaveData.CameraData, InPhase);
	UDWInputModule::Get().LoadSaveData(&SaveData.InputData, InPhase);
}

void UDWSettingModule::UnloadData(EPhase InPhase)
{
	
}

FSaveData* UDWSettingModule::ToData()
{
	static FDWSettingModuleSaveData SaveData;
	SaveData = FDWSettingModuleSaveData();

	SaveData.GameData.GameLevel = GameLevel;
	SaveData.GameData.bAutoJump = bAutoJump;

	SaveData.AudioData = UDWAudioModule::Get().GetSaveDataRef<FDWAudioModuleSaveData>(true);
	SaveData.VideoData = UDWVideoModule::Get().GetSaveDataRef<FDWVideoModuleSaveData>(true);
	SaveData.CameraData = UDWCameraModule::Get().GetSaveDataRef<FDWCameraModuleSaveData>(true);
	SaveData.InputData = UDWInputModule::Get().GetSaveDataRef<FDWInputModuleSaveData>(false);
	
	return &SaveData;
}
