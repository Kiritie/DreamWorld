// Fill out your copyright notice in the Description page of Project Settings.


#include "Video/DWVideoModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
	
IMPLEMENTATION_MODULE(UDWVideoModule)

// ParamSets default values
UDWVideoModule::UDWVideoModule()
{
}

UDWVideoModule::~UDWVideoModule()
{
	TERMINATION_MODULE(UDWVideoModule)
}

#if WITH_EDITOR
void UDWVideoModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWVideoModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void UDWVideoModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWVideoModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWVideoModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWVideoModule::OnPause()
{
	Super::OnPause();
}

void UDWVideoModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWVideoModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWVideoModuleSaveData>();
}

void UDWVideoModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWVideoModule::ToData()
{
	static FDWVideoModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVideoModuleSaveData>();

	return &SaveData;
}
