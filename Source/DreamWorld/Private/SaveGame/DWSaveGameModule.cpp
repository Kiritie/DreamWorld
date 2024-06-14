// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/DWSaveGameModule.h"

#include "SaveGame/SaveGameModuleStatics.h"
	
IMPLEMENTATION_MODULE(UDWSaveGameModule)

// ParamSets default values
UDWSaveGameModule::UDWSaveGameModule()
{
}

UDWSaveGameModule::~UDWSaveGameModule()
{
	TERMINATION_MODULE(UDWSaveGameModule)
}

#if WITH_EDITOR
void UDWSaveGameModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWSaveGameModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWSaveGameModule)
}
#endif

void UDWSaveGameModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWSaveGameModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWSaveGameModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWSaveGameModule::OnPause()
{
	Super::OnPause();
}

void UDWSaveGameModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWSaveGameModule::OnTermination(EPhase InPhase)
{
	Super::OnTermination(InPhase);
}

void UDWSaveGameModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWSaveGameModule::ToData()
{
	return Super::ToData();
}
