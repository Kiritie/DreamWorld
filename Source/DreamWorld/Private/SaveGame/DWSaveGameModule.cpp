// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/DWSaveGameModule.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
	
IMPLEMENTATION_MODULE(ADWSaveGameModule)

// ParamSets default values
ADWSaveGameModule::ADWSaveGameModule()
{
	GeneralSaveGame = UDWGeneralSaveGame::StaticClass();
}

ADWSaveGameModule::~ADWSaveGameModule()
{
	TERMINATION_MODULE(ADWSaveGameModule)
}

#if WITH_EDITOR
void ADWSaveGameModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWSaveGameModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWSaveGameModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWSaveGameModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWSaveGameModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWSaveGameModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWSaveGameModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWSaveGameModule::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

void ADWSaveGameModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWSaveGameModule::ToData()
{
	return Super::ToData();
}
