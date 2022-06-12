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
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Initializing.h"
#include "Procedure/Procedure_Loading.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "Procedure/Procedure_Starting.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

ADWGameMode::ADWGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APawn::StaticClass();
}

void ADWGameMode::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	//UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Initializing>();
}

void ADWGameMode::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();

	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Starting>();
}
