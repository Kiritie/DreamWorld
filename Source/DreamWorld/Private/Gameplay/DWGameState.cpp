// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameState.h"
#include "Engine/World.h"
#include "SaveGame/SaveGameModuleStatics.h"

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
