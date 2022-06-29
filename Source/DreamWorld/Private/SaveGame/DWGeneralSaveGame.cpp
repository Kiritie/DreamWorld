// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/DWGeneralSaveGame.h"

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

void UDWGeneralSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();
}

void UDWGeneralSaveGame::OnUnload_Implementation()
{
	Super::OnLoad_Implementation();
}

void UDWGeneralSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	SaveData.CameraDistance = UCameraModuleBPLibrary::GetCurrentCameraDistance();
}
