// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/DWTaskModule.h"

IMPLEMENTATION_MODULE(ADWTaskModule)

// ParamSets default values
ADWTaskModule::ADWTaskModule()
{
}

ADWTaskModule::~ADWTaskModule()
{
	TERMINATION_MODULE(ADWTaskModule)
}

#if WITH_EDITOR
void ADWTaskModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWTaskModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWTaskModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWTaskModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWTaskModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWTaskModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWTaskModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWTaskModule::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}

void ADWTaskModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWTaskModule::ToData()
{
	return Super::ToData();
}
