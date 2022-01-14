// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWPlayerController.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Main/MainModule.h"
#include "Module/DWSaveGameModule.h"
#include "SaveGame/ArchiveSaveGame.h"
#include "SaveGame/ArchiveSaveGameData.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

ADWGameMode::ADWGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APawn::StaticClass();
}

void ADWGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->OnModuleInitialized.AddDynamic(this, &ADWGameMode::InitializeGame);
	}
}

void ADWGameMode::InitializeGame()
{
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::MainMenu);
	}
}

void ADWGameMode::StartGame(FName InArchiveID)
{
	// if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
	// {
	// 	SaveGameModule->LoadArchiveData(InArchiveID, true);
	// }
}

void ADWGameMode::ContinueGame()
{
	// if(ADWSaveGameModule* SaveGameModule = AMainModule::GetModuleByClass<ADWSaveGameModule>())
	// {
	// 	if(UGeneralSaveGame* GeneralSaveGame = SaveGameModule->GetGeneralSaveGame())
	// 	{
	// 		SaveGameModule->LoadArchiveData(GeneralSaveGame->GeneralData.CurrentArchiveID, true);
	// 	}
	// }
}

void ADWGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::Pausing);
	}
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->PauseModules();
	}
}

void ADWGameMode::UnPauseGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::Playing);
	}
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->UnPauseModules();
	}
}

void ADWGameMode::BackMainMenu()
{
	if(UArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UArchiveSaveGame>())
	{
		if(UArchiveSaveGameData* ArchiveSaveGameData = ArchiveSaveGame->GetSaveGameData<UArchiveSaveGameData>())
		{
			USaveGameModuleBPLibrary::SaveSaveGame<UArchiveSaveGame>(ArchiveSaveGameData->ArchiveData.ID);
		}
	}
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		if (DWGameState->GetCurrentState() == EGameState::Pausing)
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
		DWGameState->SetCurrentState(EGameState::MainMenu);
	}
}

void ADWGameMode::QuitGame()
{
	if(UArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UArchiveSaveGame>())
	{
		if(UArchiveSaveGameData* ArchiveSaveGameData = ArchiveSaveGame->GetSaveGameData<UArchiveSaveGameData>())
		{
			USaveGameModuleBPLibrary::SaveSaveGame<UArchiveSaveGame>(ArchiveSaveGameData->ArchiveData.ID);
		}
	}
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
