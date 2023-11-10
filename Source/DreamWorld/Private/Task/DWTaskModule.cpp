// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/DWTaskModule.h"

#include "Common/DWCommonTypes.h"

IMPLEMENTATION_MODULE(UDWTaskModule)

// ParamSets default values
UDWTaskModule::UDWTaskModule()
{
}

UDWTaskModule::~UDWTaskModule()
{
	TERMINATION_MODULE(UDWTaskModule)
}

#if WITH_EDITOR
void UDWTaskModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTaskModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWTaskModule)
}
#endif

void UDWTaskModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTaskModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWTaskModule::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWTaskModule::OnPause()
{
	Super::OnPause();
}

void UDWTaskModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWTaskModule::OnTermination(EPhase InPhase)
{
	Super::OnTermination(InPhase);
}

void UDWTaskModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWTaskModuleSaveData>();
}

FSaveData* UDWTaskModule::ToData()
{
	static FDWTaskModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FTaskModuleSaveData>();

	return &SaveData;
}
