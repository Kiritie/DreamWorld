// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DWCameraModule.h"

#include "Common/DWCommonTypes.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
	
IMPLEMENTATION_MODULE(UDWCameraModule)

// ParamSets default values
UDWCameraModule::UDWCameraModule()
{
	bCameraMoveControlAble = false;
	CameraMoveSpeed = 6.f;
	CameraZoomSpeed = 8.f;
	MinCameraDistance = 150.f;
	InitCameraDistance = 200.f;
	MaxCameraDistance = 350.f;
}

UDWCameraModule::~UDWCameraModule()
{
	TERMINATION_MODULE(UDWCameraModule)
}

#if WITH_EDITOR
void UDWCameraModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWCameraModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWCameraModule)
}
#endif

void UDWCameraModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWCameraModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWCameraModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWCameraModule::OnPause()
{
	Super::OnPause();
}

void UDWCameraModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWCameraModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWCameraModuleSaveData>();
}

void UDWCameraModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWCameraModule::ToData()
{
	static FDWCameraModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FCameraModuleSaveData>();

	return &SaveData;
}
