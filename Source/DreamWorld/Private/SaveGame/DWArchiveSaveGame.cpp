// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/DWArchiveSaveGame.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/DWSaveGameModule.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"

UDWArchiveSaveGame::UDWArchiveSaveGame()
{
	// set default pawn class to our Blueprinted character

	SaveName = FName("Archive");
	
	SaveData = FArchiveSaveData();
}

void UDWArchiveSaveGame::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();

	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(SaveData.ID == -1)
		{
			SaveData.ID = GeneralSaveGame->SaveData.ArchiveBasicDatas.Num();
			if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
			{
				SaveData.WorldData = SaveGameModule->GetDefaultWorldData();
				SaveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
			}
			SaveData.bPreview = true;
			SaveData.Initialize();
		}
		GeneralSaveGame->SaveData.ArchiveBasicDatas.Add(SaveData);
	}
}

void UDWArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		GeneralSaveGame->SaveData.CurrentArchiveID = SaveData.ID;

		WHDebug(FString::Printf(TEXT("Loading archive : %d"), SaveData.ID), FColor::Cyan);

		if(AVoxelModule* VoxelModule = AVoxelModule::Get())
		{
			VoxelModule->LoadData(&SaveData.WorldData);
		}
		if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
		{
			PlayerController->LoadData(SaveData.PlayerData);
		}
	}
}

void UDWArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), SaveData.ID), FColor::Cyan);

	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		PlayerController->UnloadData(true);
	}
	if(AVoxelModule* VoxelModule = AVoxelModule::Get())
	{
		VoxelModule->UnloadData(true);
	}
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(ADWPlayerCharacter* PlayerCharacter = UDWHelper::GetPlayerCharacter(this))
	{
		SaveData.PlayerData = *static_cast<FPlayerSaveData*>(PlayerCharacter->ToData());
	}
	if(AVoxelModule* VoxelModule = AVoxelModule::Get())
	{
		SaveData.WorldData = *static_cast<FDWWorldSaveData*>(VoxelModule->ToData());
	}
}
