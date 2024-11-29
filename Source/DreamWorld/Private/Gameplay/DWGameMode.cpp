// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"

#include "Gameplay/DWGameState.h"
#include "Gameplay/DWHUD.h"
#include "Gameplay/DWPlayerController.h"
#include "Gameplay/DWPlayerState.h"
#include "SaveGame/SaveGameModuleStatics.h"

ADWGameMode::ADWGameMode()
{
	HUDClass = ADWHUD::StaticClass();
	PlayerControllerClass = ADWPlayerController::StaticClass();
	GameStateClass = ADWGameState::StaticClass();
	PlayerStateClass = ADWPlayerState::StaticClass();
}

void ADWGameMode::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWGameMode::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWGameMode::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWGameMode::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}
