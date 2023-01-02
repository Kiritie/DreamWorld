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
	DWArchiveSaveData.WorldData = ADWVoxelModule::Get()->GetWorldBasicData();
	DWArchiveSaveData.PlayerData = ADWCharacterModule::Get()->GetPlayerBasicData();
}

void UDWArchiveSaveGame::OnSave_Implementation()
{
	Super::OnSave_Implementation();
}

void UDWArchiveSaveGame::OnLoad_Implementation(EPhase InPhase)
{
	Super::OnLoad_Implementation(InPhase);

	WHDebug(FString::Printf(TEXT("Loading archive : %d"), DWArchiveSaveData.ID));

	AVoxelModule::Get()->LoadSaveData(&DWArchiveSaveData.WorldData, InPhase);
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&DWArchiveSaveData.PlayerData, InPhase);
}

void UDWArchiveSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);

	WHDebug(FString::Printf(TEXT("Unloading archive : %d"), DWArchiveSaveData.ID));

	ADWVoxelModule::Get()->UnloadSaveData(InPhase);
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(InPhase);
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	DWArchiveSaveData.PlayerData = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>()->ToSaveDataRef<FDWPlayerSaveData>(true);
	DWArchiveSaveData.WorldData = ADWVoxelModule::Get()->ToSaveDataRef<FDWVoxelWorldSaveData>();
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
		ADWVoxelModule::Get()->UnloadSaveData(EPhase::Primary);
		UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(EPhase::Primary);
	}
}
