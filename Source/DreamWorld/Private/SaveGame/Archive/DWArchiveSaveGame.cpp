// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Archive/DWArchiveSaveGame.h"

#include "Achievement/AchievementModule.h"
#include "Achievement/DWAchievementModule.h"
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
	
	SaveData = FDWArchiveSaveData();
}

void UDWArchiveSaveGame::OnCreate_Implementation(int32 InIndex)
{
	Super::OnCreate_Implementation(InIndex);

	SaveData.ID = InIndex;
	SaveData.WorldData = ADWVoxelModule::Get()->GetWorldBasicData();
	SaveData.PlayerData = ADWCharacterModule::Get()->GetPlayerBasicData();
	SaveData.AchievementData.States.SetNum(AAchievementModule::Get()->Achievements.Num());
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
		WHDebug(FString::Printf(TEXT("Loading archive : %d"), SaveData.ID), EDebugMode::All, EDC_SaveGame);
	}
	AVoxelModule::Get()->LoadSaveData(&SaveData.WorldData, InPhase);
	UCommonBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&SaveData.PlayerData, InPhase);
	ADWAchievementModule::Get()->LoadSaveData(&SaveData.AchievementData, InPhase);
	ADWTaskModule::Get()->LoadSaveData(&SaveData.TaskData, InPhase);
	ADWTeamModule::Get()->LoadSaveData(&SaveData.TeamData, InPhase);
}

void UDWArchiveSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);

	if(PHASEC(InPhase, EPhase::Primary))
	{
		WHDebug(FString::Printf(TEXT("Unloading archive : %d"), SaveData.ID), EDebugMode::All, EDC_SaveGame);
	}
	ADWVoxelModule::Get()->UnloadSaveData(InPhase);
	UCommonBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(InPhase);
	ADWAchievementModule::Get()->UnloadSaveData(InPhase);
	ADWTaskModule::Get()->UnloadSaveData(InPhase);
	ADWTeamModule::Get()->UnloadSaveData(InPhase);
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	SaveData.WorldData = ADWVoxelModule::Get()->GetSaveDataRef<FDWWorldSaveData>(true);
	SaveData.PlayerData = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>()->GetSaveDataRef<FDWPlayerSaveData>(true);
	SaveData.AchievementData = ADWAchievementModule::Get()->GetSaveDataRef<FDWAchievementModuleSaveData>(true);
	SaveData.TaskData = ADWTaskModule::Get()->GetSaveDataRef<FDWTaskModuleSaveData>(true);
	SaveData.TeamData = ADWTeamModule::Get()->GetSaveDataRef<FDWTeamModuleSaveData>(true);
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
