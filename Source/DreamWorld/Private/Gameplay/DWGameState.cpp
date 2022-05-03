// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Gameplay/DWGameInstance.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

ADWGameState::ADWGameState()
{
	// set default pawn class to our Blueprinted character
	
	CurrentState = EDWGameState::None;
}

void ADWGameState::BeginPlay()
{
	Super::BeginPlay();

}

void ADWGameState::SetCurrentState(EDWGameState InGameState)
{
	if (CurrentState != InGameState)
	{
		CurrentState = InGameState;
		switch (InGameState)
		{
			case EDWGameState::MainMenu:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
				
				UWidgetModuleBPLibrary::CloseUserWidget<UWidgetGameHUD>();
				UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryBar>();
				
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveChoosingPanel>();
				break;
			}
			case EDWGameState::Preparing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveChoosingPanel>();

				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveCreatingPanel>();
				break;
			}
			case EDWGameState::Loading:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>();
				
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetGameHUD>();
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryBar>();
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryPanel>();
				break;
			}
			case EDWGameState::Playing:
			{
				if(UWidgetModuleBPLibrary::GetTemporaryUserWidget<UUserWidgetBase>())
				{
					UWidgetModuleBPLibrary::GetTemporaryUserWidget<UUserWidgetBase>()->Close();
				}
				
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGameHUD>();
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryBar>();
				break;
			}
			case EDWGameState::Pausing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetPausingMenu>();
				break;
			}
			default: break;
		}
	}
}
