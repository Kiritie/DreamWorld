// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Gameplay/DWGameInstance.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
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
	
}

void ADWGameState::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWGameState::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWGameState::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWGameState::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}
