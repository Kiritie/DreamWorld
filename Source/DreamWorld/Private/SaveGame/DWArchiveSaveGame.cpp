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
#include "Voxel/DWVoxelModule.h"

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
		if(InSaveIndex == -1) SaveIndex = SaveGameModule->GetValidArchiveID();
		SaveData.ID = SaveIndex;
		SaveData.WorldData = SaveGameModule->GetDefaultWorldData();
		SaveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
		SaveData.Initialize();
	}
}

void UDWArchiveSaveGame::OnSave_Implementation()
{
	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(SaveData.ID);
	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Emplace(SaveIndex, SaveData);

	Super::OnSave_Implementation();
}

void UDWArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(SaveData.ID);

	WHDebug(FString::Printf(TEXT("Loading archive : %d"), SaveData.ID));

	USaveGameModuleBPLibrary::LoadObjectData(AMainModule::GetModuleByClass<AVoxelModule>(), &SaveData.WorldData);
	USaveGameModuleBPLibrary::LoadObjectData(UGlobalBPLibrary::GetPlayerController<ADWPlayerController>(), &SaveData.PlayerData);
}

void UDWArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), SaveData.ID));

	USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(-1);

	USaveGameModuleBPLibrary::UnloadDataObject(AMainModule::GetModuleByClass<ADWVoxelModule>());
	USaveGameModuleBPLibrary::UnloadDataObject(UGlobalBPLibrary::GetPlayerController<ADWPlayerController>());
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	SaveData.PlayerData = USaveGameModuleBPLibrary::ObjectToDataRef<FDWPlayerSaveData>(UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>(), true, true);
	SaveData.WorldData = USaveGameModuleBPLibrary::ObjectToDataRef<FDWVoxelWorldSaveData>(AMainModule::GetModuleByClass<ADWVoxelModule>());
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
	
	if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Contains(SaveIndex))
	{
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Remove(SaveIndex);
	}
}
