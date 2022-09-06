// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/DWSaveGameModule.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

// ParamSets default values
ADWSaveGameModule::ADWSaveGameModule()
{
	GeneralSaveGame = UDWGeneralSaveGame::StaticClass();
}

#if WITH_EDITOR
void ADWSaveGameModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWSaveGameModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWSaveGameModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWSaveGameModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
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

void ADWSaveGameModule::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* ADWSaveGameModule::ToData()
{
	static FDWGeneralSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FGeneralSaveData>();
	return &SaveData;
}
