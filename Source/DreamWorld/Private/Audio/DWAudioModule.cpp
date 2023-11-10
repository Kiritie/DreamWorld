// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/DWAudioModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
	
IMPLEMENTATION_MODULE(UDWAudioModule)

// ParamSets default values
UDWAudioModule::UDWAudioModule()
{
}

UDWAudioModule::~UDWAudioModule()
{
	TERMINATION_MODULE(UDWAudioModule)
}

#if WITH_EDITOR
void UDWAudioModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWAudioModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWAudioModule)
}
#endif

void UDWAudioModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWAudioModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWAudioModule::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWAudioModule::OnPause()
{
	Super::OnPause();
}

void UDWAudioModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWAudioModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWAudioModuleSaveData>();
}

void UDWAudioModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWAudioModule::ToData()
{
	static FDWAudioModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FAudioModuleSaveData>();

	return &SaveData;
}
