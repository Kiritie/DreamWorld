// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Event/EventModuleBPLibrary.h"
#include "Gameplay/DWPlayerController.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Main/MainModule.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

ADWGameMode::ADWGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APawn::StaticClass();
}

void ADWGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
}

void ADWGameMode::InitializeGame()
{
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::MainMenu);
	}
}

void ADWGameMode::StartGame(int32 InArchiveID)
{
	if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(InArchiveID))
	{
		ArchiveSaveGame->OnLoad();
	}
}

void ADWGameMode::ContinueGame()
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::LoadSaveGame<UDWGeneralSaveGame>(0))
	{
		if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(GeneralSaveGame->SaveData.CurrentArchiveID))
		{
			ArchiveSaveGame->OnLoad();
		}
	}
}

void ADWGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Pausing);
	}
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->PauseModules();
	}
}

void ADWGameMode::UnPauseGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Playing);
	}
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->UnPauseModules();
	}
}

void ADWGameMode::BackMainMenu()
{
	if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>())
	{
		USaveGameModuleBPLibrary::SaveSaveGame<UDWArchiveSaveGame>(ArchiveSaveGame->SaveData.ID);
	}
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if (DWGameState->GetCurrentState() == EDWGameState::Pausing)
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
		DWGameState->SetCurrentState(EDWGameState::MainMenu);
	}
}

void ADWGameMode::QuitGame()
{
	if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>())
	{
		USaveGameModuleBPLibrary::SaveSaveGame<UDWArchiveSaveGame>(ArchiveSaveGame->SaveData.ID);
	}
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
