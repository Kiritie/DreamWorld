// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/ArchiveSaveGame.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "Module/DWSaveGameModule.h"
#include "SaveGame/ArchiveSaveGameData.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "World/WorldManager.h"

UArchiveSaveGame::UArchiveSaveGame()
{
	// set default pawn class to our Blueprinted character
	
}

void UArchiveSaveGame::OnCreate_Implementation(USaveGameDataBase* InSaveGameData)
{
	Super::OnCreate_Implementation(InSaveGameData);

	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		if(UArchiveSaveGameData* ArchiveSaveGameData = GetSaveGameData<UArchiveSaveGameData>())
		{
			if(ArchiveSaveGameData->ArchiveData.ID == -1)
			{
				ArchiveSaveGameData->ArchiveData.ID = GeneralSaveGame->ArchiveBasicDatas.Num() - 1;
				if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
				{
					ArchiveSaveGameData->ArchiveData.WorldData = SaveGameModule->GetDefaultWorldData();
					ArchiveSaveGameData->ArchiveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
				}
				ArchiveSaveGameData->ArchiveData.bPreview = true;
				ArchiveSaveGameData->ArchiveData.Initialize();
			}
			GeneralSaveGame->ArchiveBasicDatas.Add(ArchiveSaveGameData->ArchiveData);
		}
	}
}

void UArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		if(UArchiveSaveGameData* ArchiveSaveGameData = GetSaveGameData<UArchiveSaveGameData>())
		{
			GeneralSaveGame->GeneralData.CurrentArchiveID = ArchiveSaveGameData->ArchiveData.ID;

			WHDebug(FString::Printf(TEXT("Loading archive : %d"), ArchiveSaveGameData->ArchiveData.ID), FColor::Cyan);
	
			if(AWorldManager* WorldManager = AWorldManager::Get())
			{
				WorldManager->LoadData(ArchiveSaveGameData->ArchiveData.WorldData);
			}
			if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
			{
				PlayerController->LoadData(ArchiveSaveGameData->ArchiveData.PlayerData);
			}
		}
	}
}

void UArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	if(UArchiveSaveGameData* ArchiveSaveGameData = GetSaveGameData<UArchiveSaveGameData>())
	{
		WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), ArchiveSaveGameData->ArchiveData.ID), FColor::Cyan);

		if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
		{
			PlayerController->UnloadData(true);
		}
		if(AWorldManager* WorldManager = AWorldManager::Get())
		{
			WorldManager->UnloadData(true);
		}
	}
}

void UArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(UArchiveSaveGameData* ArchiveSaveGameData = GetSaveGameData<UArchiveSaveGameData>())
	{
		if(ADWPlayerCharacter* PlayerCharacter = UDWHelper::GetPlayerCharacter(this))
		{
			ArchiveSaveGameData->ArchiveData.PlayerData = *static_cast<FPlayerSaveData*>(PlayerCharacter->ToData());
		}
		if(AWorldManager* WorldManager = AWorldManager::Get())
		{
			ArchiveSaveGameData->ArchiveData.WorldData = WorldManager->ToData();
		}
	}
}

UArchiveSaveGameData::UArchiveSaveGameData()
{
	ArchiveData = FArchiveSaveData();
}

void UArchiveSaveGameData::OnInitialize_Implementation()
{
	
}
