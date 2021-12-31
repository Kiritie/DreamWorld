// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Gameplay/DWGameInstance.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/WidgetPrimaryPanel.h"
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
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::SemiPermanent);
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetMainMenu>());
				break;
			}
			case EGameState::Preparing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveChoosingPanel>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetArchiveChoosingPanel>());
				break;
			}
			case EGameState::Loading:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetLoadingPanel>());
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetPrimaryPanel>(UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetPrimaryPanel>());
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryBar>(UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetInventoryBar>());
				UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryPanel>(UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetInventoryPanel>());
				break;
			}
			case EGameState::Playing:
			{
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::Temporary);
				UWidgetModuleBPLibrary::CloseAllUserWidget(EWidgetType::SemiTemporary);
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetPrimaryPanel>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetPrimaryPanel>());
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryBar>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetInventoryBar>());
				break;
			}
			case EGameState::Pausing:
			{
				UWidgetModuleBPLibrary::OpenUserWidget<UWidgetPausingMenu>(false, UWidgetModuleBPLibrary::GetUserWidgetClass<UWidgetPausingMenu>());
				break;
			}
			default: break;
		}
	}
}
