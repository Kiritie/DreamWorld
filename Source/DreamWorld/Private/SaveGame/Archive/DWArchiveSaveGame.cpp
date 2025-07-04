// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Archive/DWArchiveSaveGame.h"

#include "Achievement/AchievementModule.h"
#include "Character/DWCharacterModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Debug/DebugModuleTypes.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Task/TaskModule.h"
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
	SaveData.WorldData = UDWVoxelModule::Get().GetWorldBasicData();
	SaveData.PlayerData = UDWCharacterModule::Get().GetPlayerBasicData();
	SaveData.AchievementData.States.SetNum(UAchievementModule::Get().Achievements.Num());
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
		WHDebug(FString::Printf(TEXT("Loading archive : %d"), SaveData.ID), EDM_All, EDC_SaveGame);
	}
	UDWVoxelModule::Get().LoadSaveData(&SaveData.WorldData, InPhase);
	UCommonStatics::GetPlayerController<ADWPlayerController>()->LoadSaveData(&SaveData.PlayerData, InPhase);
	UAchievementModule::Get().LoadSaveData(&SaveData.AchievementData, InPhase);
	UTaskModule::Get().LoadSaveData(&SaveData.TaskData, InPhase);
	UDWTeamModule::Get().LoadSaveData(&SaveData.TeamData, InPhase);
}

void UDWArchiveSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);

	if(PHASEC(InPhase, EPhase::Primary))
	{
		WHDebug(FString::Printf(TEXT("Unloading archive : %d"), SaveData.ID), EDM_All, EDC_SaveGame);
	}
	UDWVoxelModule::Get().UnloadSaveData(InPhase);
	UCommonStatics::GetPlayerController<ADWPlayerController>()->UnloadSaveData(InPhase);
	UAchievementModule::Get().UnloadSaveData(InPhase);
	UTaskModule::Get().UnloadSaveData(InPhase);
	UDWTeamModule::Get().UnloadSaveData(InPhase);
}

void UDWArchiveSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	SaveData.WorldData = UDWVoxelModule::Get().GetSaveDataRef<FDWWorldSaveData>(true);
	SaveData.PlayerData = UCommonStatics::GetPlayerController<ADWPlayerController>()->GetSaveDataRef<FDWPlayerSaveData>(true);
	SaveData.AchievementData = UAchievementModule::Get().GetSaveDataRef<FAchievementModuleSaveData>(true);
	SaveData.TaskData = UTaskModule::Get().GetSaveDataRef<FTaskModuleSaveData>(true);
	SaveData.TeamData = UDWTeamModule::Get().GetSaveDataRef<FDWTeamModuleSaveData>(true);
}

void UDWArchiveSaveGame::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
