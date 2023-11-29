// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/Setting/DWSettingSaveGame.h"

#include "Common/CommonStatics.h"
#include "Setting/DWSettingModule.h"

UDWSettingSaveGame::UDWSettingSaveGame()
{
	SaveName = FName("Setting");

	ModuleClass = UDWSettingModule::StaticClass();
}

void UDWSettingSaveGame::OnCreate_Implementation(int32 InIndex)
{
	Super::OnCreate_Implementation(InIndex);
}

void UDWSettingSaveGame::OnLoad_Implementation(EPhase InPhase)
{
	Super::OnLoad_Implementation(InPhase);
}

void UDWSettingSaveGame::OnUnload_Implementation(EPhase InPhase)
{
	Super::OnUnload_Implementation(InPhase);
}

void UDWSettingSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
