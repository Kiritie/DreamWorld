// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/DWAudioModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWAudioModule)

// ParamSets default values
ADWAudioModule::ADWAudioModule()
{
}

ADWAudioModule::~ADWAudioModule()
{
	TERMINATION_MODULE(ADWAudioModule)
}

#if WITH_EDITOR
void ADWAudioModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWAudioModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWAudioModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWAudioModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWAudioModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWAudioModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWAudioModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWAudioModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWAudioModuleSaveData>();
}

void ADWAudioModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* ADWAudioModule::ToData()
{
	static FDWAudioModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FAudioModuleSaveData>();

	return &SaveData;
}