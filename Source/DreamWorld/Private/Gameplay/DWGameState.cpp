// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Gameplay/DWGameInstance.h"
#include "Module/DWSaveGameModule.h"
#include "SaveGame/GeneralSaveGame.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

ADWGameState::ADWGameState()
{
	// set default pawn class to our Blueprinted character
	
	CurrentState = EGameState::None;
}

void ADWGameState::BeginPlay()
{
	Super::BeginPlay();

}

void ADWGameState::SetCurrentState(EGameState InGameState)
{
	if (CurrentState != InGameState)
	{
		CurrentState = InGameState;
		switch (InGameState)
		{
			case EGameState::MainMenu:
			{
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::SemiPermanent, true);
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::Temporary, true);
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::SemiTemporary, true);
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
				break;
			}
			case EGameState::Preparing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveChoosingPanel>();
				break;
			}
			case EGameState::Loading:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>();
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetGameHUD>();
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryBar>();
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryPanel>();
				break;
			}
			case EGameState::Playing:
			{
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::Temporary, true);
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGameHUD>();
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryBar>();
				break;
			}
			case EGameState::Pausing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetPausingMenu>();
				break;
			}
			default: break;
		}
	}
}
