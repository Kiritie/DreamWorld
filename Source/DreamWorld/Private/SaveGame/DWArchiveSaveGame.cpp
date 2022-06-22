// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/DWArchiveSaveGame.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/DWSaveGameModule.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"

UDWArchiveSaveGame::UDWArchiveSaveGame()
{
	SaveName = FName("Archive");
	
	SaveData = FDWArchiveSaveData();
}

void UDWArchiveSaveGame::OnCreate_Implementation(int32 InSaveIndex)
{
	Super::OnCreate_Implementation(InSaveIndex);

	if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
	{
		SaveData.WorldData = SaveGameModule->GetDefaultWorldData();
		SaveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
		SaveData.Initialize();
	}
}

void UDWArchiveSaveGame::OnSave_Implementation()
{
	if(IsPreview())
	{
		SaveIndex = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetValidArchiveID();
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(SaveData.ID = SaveIndex);
		SaveData.Initialize();
	}
	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Emplace(SaveIndex, SaveData);

	Super::OnSave_Implementation();
}

void UDWArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(SaveData.ID);

	WHDebug(FString::Printf(TEXT("Loading archive : %d"), SaveData.ID));

	if(AVoxelModule* VoxelModule = AMainModule::GetModuleByClass<AVoxelModule>())
	{
		VoxelModule->LoadData(&SaveData.WorldData);
	}
	if(ADWPlayerController* PlayerController = UGlobalBPLibrary::GetPlayerController<ADWPlayerController>())
	{
		PlayerController->LoadData(SaveData.PlayerData);
	}
}

void UDWArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), SaveData.ID));

	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(-1);
	if(AVoxelModule* VoxelModule = AMainModule::GetModuleByClass<AVoxelModule>())
	{
		VoxelModule->UnloadData();
	}
	if(ADWPlayerController* PlayerController = UGlobalBPLibrary::GetPlayerController<ADWPlayerController>())
	{
		PlayerController->UnloadData();
	}
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
	{
		SaveData.PlayerData = *static_cast<FDWPlayerSaveData*>(PlayerCharacter->ToData());
	}
	if(AVoxelModule* VoxelModule = AMainModule::GetModuleByClass<AVoxelModule>())
	{
		SaveData.WorldData = *static_cast<FDWVoxelWorldSaveData*>(VoxelModule->ToData());
	}
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
	
	if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Contains(SaveIndex))
	{
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Remove(SaveIndex);
	}
}
