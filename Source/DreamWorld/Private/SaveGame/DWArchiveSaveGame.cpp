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
	
	DWArchiveSaveData = FDWArchiveSaveData();
}

void UDWArchiveSaveGame::OnCreate_Implementation(int32 InSaveIndex)
{
	Super::OnCreate_Implementation(InSaveIndex);

	if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
	{
		DWArchiveSaveData.ID = InSaveIndex;
		DWArchiveSaveData.WorldData = SaveGameModule->GetDefaultWorldData();
		DWArchiveSaveData.PlayerData = SaveGameModule->GetDefaultPlayerData();
		DWArchiveSaveData.Initialize();
	}
}

void UDWArchiveSaveGame::OnSave_Implementation()
{
	Super::OnSave_Implementation();
}

void UDWArchiveSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();

	WHDebug(FString::Printf(TEXT("Loading archive : %d"), DWArchiveSaveData.ID));

	USaveGameModuleBPLibrary::LoadObjectData(AMainModule::GetModuleByClass<AVoxelModule>(), &DWArchiveSaveData.WorldData);
	USaveGameModuleBPLibrary::LoadObjectData(UGlobalBPLibrary::GetPlayerController<ADWPlayerController>(), &DWArchiveSaveData.PlayerData);
}

void UDWArchiveSaveGame::OnUnload_Implementation()
{
	Super::OnUnload_Implementation();

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), DWArchiveSaveData.ID));

	USaveGameModuleBPLibrary::UnloadDataObject(AMainModule::GetModuleByClass<ADWVoxelModule>());
	USaveGameModuleBPLibrary::UnloadDataObject(UGlobalBPLibrary::GetPlayerController<ADWPlayerController>());
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	DWArchiveSaveData.PlayerData = USaveGameModuleBPLibrary::ObjectToDataRef<FDWPlayerSaveData>(UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>(), true, true);
	DWArchiveSaveData.WorldData = USaveGameModuleBPLibrary::ObjectToDataRef<FDWVoxelWorldSaveData>(AMainModule::GetModuleByClass<ADWVoxelModule>());
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
