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
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::LoadSaveGame<UDWGeneralSaveGame>())
	{
		GeneralSaveGame->SaveData.CurrentArchiveID = InArchiveID;
	}
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Loading);
	}
}

void ADWGameMode::ContinueGame()
{
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Loading);
	}
}

void ADWGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->PauseModules();
	}
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Pausing);
	}
}

void ADWGameMode::UnPauseGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->UnPauseModules();
	}
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		DWGameState->SetCurrentState(EDWGameState::Playing);
	}
}

void ADWGameMode::BackMainMenu()
{
	USaveGameModuleBPLibrary::SaveSaveGame<UDWArchiveSaveGame>();
	if(ADWGameState* DWGameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(DWGameState->GetCurrentState() == EDWGameState::Pausing)
		{
			UnPauseGame();
		}
		DWGameState->SetCurrentState(EDWGameState::MainMenu);
	}
}

void ADWGameMode::QuitGame()
{
	USaveGameModuleBPLibrary::SaveSaveGame<UDWArchiveSaveGame>();
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
