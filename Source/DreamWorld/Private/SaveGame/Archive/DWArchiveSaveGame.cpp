// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
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

	AMainModule::GetModuleByClass<AVoxelModule>()->LoadSaveData(&DWArchiveSaveData.WorldData);
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&DWArchiveSaveData.PlayerData);
}

void UDWArchiveSaveGame::OnUnload_Implementation(bool bForceMode)
{
	Super::OnUnload_Implementation(bForceMode);

	WHDebug(FString::Printf(TEXT("UnLoading archive : %d"), DWArchiveSaveData.ID));

	AMainModule::GetModuleByClass<ADWVoxelModule>()->UnloadSaveData(bForceMode);
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(bForceMode);
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	DWArchiveSaveData.PlayerData = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>()->ToSaveDataRef<FDWPlayerSaveData>(true);
	DWArchiveSaveData.WorldData = AMainModule::GetModuleByClass<ADWVoxelModule>()->ToSaveDataRef<FDWVoxelWorldSaveData>();
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
