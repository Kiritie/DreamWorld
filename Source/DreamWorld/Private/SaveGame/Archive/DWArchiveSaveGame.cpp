// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Archive/DWArchiveSaveGame.h"

#include "Character/DWCharacterModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Task/DWTaskModule.h"
#include "Team/DWTeamModule.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/DWVoxelModule.h"

UDWArchiveSaveGame::UDWArchiveSaveGame()
{
	SaveName = FName("Archive");
	
	DWArchiveSaveData = FDWArchiveSaveData();
}

void UDWArchiveSaveGame::OnCreate_Implementation(int32 InIndex)
{
	Super::OnCreate_Implementation(InIndex);

	DWArchiveSaveData.ID = InIndex;
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

	if(PHASEC(InPhase, EPhase::Primary))
	{
		WHDebug(FString::Printf(TEXT("Loading archive : %d"), DWArchiveSaveData.ID), EDebugMode::All, EDC_SaveGame);
	}
	AVoxelModule::Get()->LoadSaveData(&DWArchiveSaveData.WorldData, InPhase);
	UCommonBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&DWArchiveSaveData.PlayerData, InPhase);
	ADWTaskModule::Get()->LoadSaveData(&DWArchiveSaveData.TaskData, InPhase);
	ADWTeamModule::Get()->LoadSaveData(&DWArchiveSaveData.TeamData, InPhase);
}

void UDWArchiveSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);

	if(PHASEC(InPhase, EPhase::Primary))
	{
		WHDebug(FString::Printf(TEXT("Unloading archive : %d"), DWArchiveSaveData.ID), EDebugMode::All, EDC_SaveGame);
	}
	ADWVoxelModule::Get()->UnloadSaveData(InPhase);
	UCommonBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(InPhase);
	ADWTaskModule::Get()->UnloadSaveData(InPhase);
	ADWTeamModule::Get()->UnloadSaveData(InPhase);
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	DWArchiveSaveData.WorldData = ADWVoxelModule::Get()->GetSaveDataRef<FDWWorldSaveData>(true);
	DWArchiveSaveData.PlayerData = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>()->GetSaveDataRef<FDWPlayerSaveData>(true);
	DWArchiveSaveData.TaskData = ADWTaskModule::Get()->GetSaveDataRef<FDWTaskModuleSaveData>(true);
	DWArchiveSaveData.TeamData = ADWTeamModule::Get()->GetSaveDataRef<FDWTeamModuleSaveData>(true);
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
