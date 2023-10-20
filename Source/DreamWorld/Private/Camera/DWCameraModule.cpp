// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DWCameraModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWCameraModule)

// ParamSets default values
ADWCameraModule::ADWCameraModule()
{
	bCameraMoveControlAble = false;
	CameraZoomSpeed = 8.f;
	MinCameraDistance = 150.f;
	InitCameraDistance = 200.f;
	MaxCameraDistance = 350.f;
}

ADWCameraModule::~ADWCameraModule()
{
	TERMINATION_MODULE(ADWCameraModule)
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

void ADWCameraModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWCameraModuleSaveData>();
}

void ADWCameraModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* ADWCameraModule::ToData()
{
	static FDWCameraModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FCameraModuleSaveData>();

	return &SaveData;
}