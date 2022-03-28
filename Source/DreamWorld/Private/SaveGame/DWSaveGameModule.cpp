// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/DWSaveGameModule.h"

#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

// ParamSets default values
ADWSaveGameModule::ADWSaveGameModule()
{
	ArchiveBasicData = FArchiveBasicSaveData();
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

	if(USaveGameModuleBPLibrary::HasSaveGame<UDWGeneralSaveGame>(UserIndex))
	{
		USaveGameModuleBPLibrary::LoadSaveGame<UDWGeneralSaveGame>(UserIndex);
	}
	else
	{
		USaveGameModuleBPLibrary::CreateSaveGame<UDWGeneralSaveGame>();
	}
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

int32 ADWSaveGameModule::GetNewValidArchiveID() const
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		return GeneralSaveGame->SaveData.ArchiveBasicDatas.Num() - 1;
	}
	return -1;
}

FPlayerSaveData ADWSaveGameModule::GetDefaultPlayerData() const
{
	return FPlayerSaveData(ArchiveBasicData.PlayerBasicData);
}

FDWWorldSaveData ADWSaveGameModule::GetDefaultWorldData() const
{
	return FDWWorldSaveData(ArchiveBasicData.WorldBasicData);
}
