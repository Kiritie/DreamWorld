// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/ArchiveSaveGame.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "Module/DWSaveGameModule.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "World/WorldManager.h"

UArchiveSaveGame::UArchiveSaveGame()
{
	// set default pawn class to our Blueprinted character

	ArchiveData = FArchiveSaveData();
}

void UArchiveSaveGame::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();

	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		if(ArchiveData.ID == -1)
		{
			ArchiveData.ID = GeneralSaveGame->ArchiveBasicDatas.Num() - 1;
			if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
			{
				ArchiveData.WorldData = SaveGameModule->GetDefaultWorldData();
				ArchiveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
			}
			ArchiveData.bPreview = true;
			ArchiveData.Initialize();
		}
		GeneralSaveGame->ArchiveBasicDatas.Add(ArchiveData);
}
}

void UArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		GeneralSaveGame->GeneralData.CurrentArchiveID = ArchiveData.ID;

		WHDebug(FString::Printf(TEXT("Loading archive : %d"), ArchiveData.ID), FColor::Cyan);

		if(AWorldManager* WorldManager = AWorldManager::Get())
		{
			WorldManager->LoadData(ArchiveData.WorldData);
		}
		if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
		{
			PlayerController->LoadData(ArchiveData.PlayerData);
		}
	}
}

void UArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), ArchiveData.ID), FColor::Cyan);

	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		PlayerController->UnloadData(true);
	}
	if(AWorldManager* WorldManager = AWorldManager::Get())
	{
		WorldManager->UnloadData(true);
	}
}

void UArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(ADWPlayerCharacter* PlayerCharacter = UDWHelper::GetPlayerCharacter(this))
	{
		ArchiveData.PlayerData = *static_cast<FPlayerSaveData*>(PlayerCharacter->ToData());
	}
	if(AWorldManager* WorldManager = AWorldManager::Get())
	{
		ArchiveData.WorldData = WorldManager->ToData();
	}
}
