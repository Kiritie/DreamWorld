// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievement/DWAchievementModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWAchievementModule)

// ParamSets default values
ADWAchievementModule::ADWAchievementModule()
{
}

ADWAchievementModule::~ADWAchievementModule()
{
	TERMINATION_MODULE(ADWAchievementModule)
}

#if WITH_EDITOR
void ADWAchievementModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWAchievementModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWAchievementModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWAchievementModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWAchievementModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWAchievementModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWAchievementModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWAchievementModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWAchievementModuleSaveData>();
}

void ADWAchievementModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* ADWAchievementModule::ToData()
{
	static FDWAchievementModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FAchievementModuleSaveData>();

	return &SaveData;
}