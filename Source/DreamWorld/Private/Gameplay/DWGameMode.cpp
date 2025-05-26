// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameMode.h"

#include "Gameplay/DWGameState.h"
#include "Gameplay/DWHUD.h"
#include "Gameplay/DWPlayerController.h"
#include "Gameplay/DWPlayerState.h"

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

void ADWGameMode::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWGameMode::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWGameMode::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}
