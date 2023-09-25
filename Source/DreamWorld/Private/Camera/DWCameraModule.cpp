// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DWCameraModule.h"

#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
	
IMPLEMENTATION_MODULE(ADWCameraModule)

// ParamSets default values
ADWCameraModule::ADWCameraModule()
{
	bCameraControlAble = true;
	bCameraMoveAble = true;
	bCameraMoveControlAble = false;
	MaxCameraDistance = 300.f;
	InitCameraDistance = 200.f;
}

#if WITH_EDITOR
void ADWCameraModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWCameraModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWCameraModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWCameraModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWCameraModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWCameraModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWCameraModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}
