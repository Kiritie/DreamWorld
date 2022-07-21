// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/General/DWGeneralSaveGame.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"

UDWGeneralSaveGame::UDWGeneralSaveGame()
{
	SaveName = FName("General");
}

void UDWGeneralSaveGame::OnCreate_Implementation(int32 InSaveIndex)
{
	Super::OnCreate_Implementation(InSaveIndex);
}

void UDWGeneralSaveGame::OnLoad_Implementation(bool bForceMode)
{
	Super::OnLoad_Implementation(bForceMode);
}

void UDWGeneralSaveGame::OnUnload_Implementation(bool bForceMode)
{
	Super::OnUnload_Implementation(bForceMode);
}

void UDWGeneralSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
