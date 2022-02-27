// Fill out your copyright notice in the Description page of Project Settings.


#include "Module/DWSaveGameModule.h"

#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/ArchiveSaveGame.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "World/WorldManager.h"

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

	if(USaveGameModuleBPLibrary::HasSaveGame<UGeneralSaveGame>(UserIndex))
	{
		USaveGameModuleBPLibrary::LoadSaveGame<UGeneralSaveGame>(UserIndex);
	}
	else
	{
		USaveGameModuleBPLibrary::CreateSaveGame<UGeneralSaveGame>();
	}
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
	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		return GeneralSaveGame->ArchiveBasicDatas.Num() - 1;
	}
	return -1;
}

FPlayerSaveData ADWSaveGameModule::GetDefaultPlayerData() const
{
	return FPlayerSaveData(ArchiveBasicData.PlayerBasicData);
}

FWorldSaveData ADWSaveGameModule::GetDefaultWorldData() const
{
	return FWorldSaveData(ArchiveBasicData.WorldBasicData);
}
