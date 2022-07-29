// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Archive/DWArchiveSaveGame.h"

#include "Character/DWCharacterModule.h"
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

	DWArchiveSaveData.ID = InSaveIndex;
	DWArchiveSaveData.WorldData = AMainModule::GetModuleByClass<ADWVoxelModule>()->GetWorldBasicData();
	if(AMainModule::GetModuleByClass<ADWCharacterModule>())
	{
		DWArchiveSaveData.PlayerData = AMainModule::GetModuleByClass<ADWCharacterModule>()->GetPlayerBasicData();
	}
}

void UDWArchiveSaveGame::OnSave_Implementation()
{
	Super::OnSave_Implementation();
}

void UDWArchiveSaveGame::OnLoad_Implementation(bool bForceMode)
{
	Super::OnLoad_Implementation(bForceMode);

	WHDebug(FString::Printf(TEXT("Loading archive : %d"), DWArchiveSaveData.ID));

	AMainModule::GetModuleByClass<AVoxelModule>()->LoadSaveData(&DWArchiveSaveData.WorldData, bForceMode);
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&DWArchiveSaveData.PlayerData, bForceMode);
}

void UDWArchiveSaveGame::OnUnload_Implementation(bool bForceMode)
{
	Super::OnUnload_Implementation(bForceMode);

	WHDebug(FString::Printf(TEXT("Unloading archive : %d"), DWArchiveSaveData.ID));

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

void UDWArchiveSaveGame::OnActiveChange_Implementation(bool bActive)
{
	Super::OnActiveChange_Implementation(bActive);

	if(!bActive)
	{
		AMainModule::GetModuleByClass<ADWVoxelModule>()->UnloadSaveData(true);
		UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(true);
	}
}
