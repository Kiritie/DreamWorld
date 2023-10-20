// Fill out your copyright notice in the Description page of Project Settings.


#include "Video/DWVideoModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWVideoModule)

// ParamSets default values
ADWVideoModule::ADWVideoModule()
{
}

ADWVideoModule::~ADWVideoModule()
{
	TERMINATION_MODULE(ADWVideoModule)
}

#if WITH_EDITOR
void ADWVideoModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWVideoModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWVideoModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWVideoModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWVideoModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWVideoModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWVideoModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWVideoModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWVideoModuleSaveData>();
}

void ADWVideoModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* ADWVideoModule::ToData()
{
	static FDWVideoModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVideoModuleSaveData>();

	return &SaveData;
}
