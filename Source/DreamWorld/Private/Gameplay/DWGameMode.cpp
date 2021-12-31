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
#include "SaveGame/ArchiveSaveGame.h"

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
	UDWHelper::GetGameInstance(this)->InitializeData();

	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::MainMenu);
	}

	if(UArchiveSaveGame* SaveGameArchive = UDWHelper::GetGameInstance(this)->LoadArchiveData())
	{
		if(AWorldManager* WorldManager = AWorldManager::Get())
		{
			WorldManager->LoadWorld(SaveGameArchive->GetArchiveData().WorldData, true);
		}
		if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
		{
			PlayerController->LoadPlayer(SaveGameArchive->GetArchiveData().PlayerData);
		}
	}
}

void ADWGameMode::StartGame(FName InArchiveID)
{
	if(UArchiveSaveGame* SaveGameArchive = UDWHelper::GetGameInstance(this)->LoadArchiveData(InArchiveID))
	{
		if(AWorldManager* WorldManager = AWorldManager::Get())
		{
			WorldManager->LoadWorld(SaveGameArchive->GetArchiveData().WorldData, false);
		}
		if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
		{
			PlayerController->LoadPlayer(SaveGameArchive->GetArchiveData().PlayerData);
		}
	}
}

void ADWGameMode::ContinueGame()
{
	if(UDWGameInstance* GameInstance = UDWHelper::GetGameInstance(this))
	{
		StartGame(GameInstance->GetCurrentArchiveID());
	}
}

void ADWGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::Pausing);
	}
}

void ADWGameMode::UnPauseGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	if(ADWGameState* DWGameState = UDWHelper::GetGameState(this))
	{
		DWGameState->SetCurrentState(EGameState::Playing);
	}
}

void ADWGameMode::BackMainMenu()
{
	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		PlayerController->UnLoadPlayer();
	}
	if(AWorldManager* WorldManager = AWorldManager::Get())
	{
		WorldManager->UnloadWorld(true);
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
	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		PlayerController->UnLoadPlayer();
	}
	if(AWorldManager* WorldManager = AWorldManager::Get())
	{
		WorldManager->UnloadWorld();
	}
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
