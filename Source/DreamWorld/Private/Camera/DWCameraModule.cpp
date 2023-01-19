// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DWCameraModule.h"

#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
	
MODULE_INSTANCE_IMPLEMENTATION(ADWCameraModule, false)

// ParamSets default values
ADWCameraModule::ADWCameraModule()
{
	bCameraControlAble = true;
	bCameraMoveAble = true;
	bCameraMoveControlAble = false;
	CameraZoomKey = FKey("Left Ctrl");
	MaxCameraDistance = 300.f;
	InitCameraDistance = 200.f;
}

#if WITH_EDITOR
void ADWCameraModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWCameraModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
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

void ADWCameraModule::SetCameraDistance(float InDistance, bool bInstant)
{
	Super::SetCameraDistance(InDistance, bInstant);

	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCameraDistance(InDistance);
	}
}
