// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Gameplay/DWGameInstance.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
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

void ADWGameState::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWGameState::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWGameState::SetCurrentState(EDWGameState InGameState)
{
	CurrentState = InGameState;
}
