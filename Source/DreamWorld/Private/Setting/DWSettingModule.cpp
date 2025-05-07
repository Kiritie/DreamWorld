// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/DWSettingModule.h"

#include "Audio/AudioModule.h"
#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Common/DWCommonTypes.h"
#include "Input/InputModule.h"
#include "Parameter/ParameterModule.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Video/VideoModule.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetModule.h"

IMPLEMENTATION_MODULE(UDWSettingModule)

// ParamSets default values
UDWSettingModule::UDWSettingModule()
{
	ModuleSaveGame = UDWSettingSaveGame::StaticClass();

	GameLevel = EDWGameLevel::Normal;
	bAutoJump = true;
	bAutoJumpWithCreating = false;
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
	SetAutoJumpWithCreating(SaveData.GameData.bAutoJumpWithCreating);

	UWidgetModule::Get().LoadSaveData(&SaveData.WidgetData, InPhase);
	UAudioModule::Get().LoadSaveData(&SaveData.AudioData, InPhase);
	UVideoModule::Get().LoadSaveData(&SaveData.VideoData, InPhase);
	UVoxelModule::Get().GetWorldData().WorldRange = FVector2D(SaveData.VideoData.VoxelWorldRange);

	UCameraModule::Get().LoadSaveData(&SaveData.CameraData, InPhase);
	UInputModule::Get().LoadSaveData(&SaveData.InputData, InPhase);
	UParameterModule::Get().LoadSaveData(&SaveData.ParameterData, InPhase);
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
	SaveData.GameData.bAutoJumpWithCreating = bAutoJumpWithCreating;
	
	SaveData.WidgetData = UWidgetModule::Get().GetSaveDataRef<FWidgetModuleSaveData>(true);
	SaveData.AudioData = UAudioModule::Get().GetSaveDataRef<FAudioModuleSaveData>(true);
	SaveData.VideoData = UVideoModule::Get().GetSaveDataRef<FVideoModuleSaveData>(true);
	SaveData.VideoData.VoxelWorldRange = UVoxelModule::Get().GetWorldData().WorldRange.X;

	SaveData.CameraData = UCameraModule::Get().GetSaveDataRef<FCameraModuleSaveData>(true);
	SaveData.InputData = UInputModule::Get().GetSaveDataRef<FInputModuleSaveData>(false);
	SaveData.ParameterData = UParameterModule::Get().GetSaveDataRef<FParameterModuleSaveData>(true);
	
	return &SaveData;
}

bool UDWSettingModule::IsAutoJump(bool bCheckCreating) const
{
	bool ReturnValue = bAutoJump;
	if(bCheckCreating)
	{
		if(ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>())
		{
			ReturnValue = ReturnValue && PlayerCharacter->GetControlMode() != EDWCharacterControlMode::Creating || bAutoJumpWithCreating;	
		}
	}
	return ReturnValue;
}
