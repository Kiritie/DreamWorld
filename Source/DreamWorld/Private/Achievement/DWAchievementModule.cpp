// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievement/DWAchievementModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
	
IMPLEMENTATION_MODULE(UDWAchievementModule)

// ParamSets default values
UDWAchievementModule::UDWAchievementModule()
{
}

UDWAchievementModule::~UDWAchievementModule()
{
	TERMINATION_MODULE(UDWAchievementModule)
}

#if WITH_EDITOR
void UDWAchievementModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWAchievementModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWAchievementModule)
}
#endif

void UDWAchievementModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWAchievementModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWAchievementModule::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWAchievementModule::OnPause()
{
	Super::OnPause();
}

void UDWAchievementModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWAchievementModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWAchievementModuleSaveData>();
}

void UDWAchievementModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWAchievementModule::ToData()
{
	static FDWAchievementModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FAchievementModuleSaveData>();

	return &SaveData;
}
