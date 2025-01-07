// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/DWSettingModule.h"

#include "Audio/AudioModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Common/DWCommonTypes.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

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
	Super::LoadData(InSaveData, InPhase);
	
	auto& SaveData = InSaveData->CastRef<FDWSettingModuleSaveData>();

	SetGameLevel(SaveData.GameData.GameLevel);
	SetAutoJump(SaveData.GameData.bAutoJump);
	SetAutoJumpWithCreating(SaveData.GameData.bAutoJumpWithCreating);
}

void UDWSettingModule::UnloadData(EPhase InPhase)
{
	
}

FSaveData* UDWSettingModule::ToData()
{
	static FDWSettingModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FSettingModuleSaveData>();

	SaveData.GameData.GameLevel = GameLevel;
	SaveData.GameData.bAutoJump = bAutoJump;
	SaveData.GameData.bAutoJumpWithCreating = bAutoJumpWithCreating;
	
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
