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

	ArchiveBasicData = FDWArchiveBasicSaveData();
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

void ADWSaveGameModule::LoadData(FSaveData* InSaveData)
{
	Super::LoadData(InSaveData);
}

FSaveData* ADWSaveGameModule::ToData()
{
	static FDWGeneralSaveData SaveData;
	SaveData.AllSaveGameInfo = Super::ToData()->CastRef<FGeneralSaveData>().AllSaveGameInfo;
	SaveData.CameraDistance = UCameraModuleBPLibrary::GetCurrentCameraDistance();
	return &SaveData;
}

FDWPlayerSaveData ADWSaveGameModule::GetDefaultPlayerData() const
{
	return FDWPlayerSaveData(ArchiveBasicData.PlayerBasicData);
}

FDWVoxelWorldSaveData ADWSaveGameModule::GetDefaultWorldData() const
{
	return FDWVoxelWorldSaveData(ArchiveBasicData.WorldBasicData);
}
