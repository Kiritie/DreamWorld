// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/GeneralSaveGame.h"

#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"

UGeneralSaveGame::UGeneralSaveGame()
{
	SaveName = FName("General");
}

void UGeneralSaveGame::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();
}

void UGeneralSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();
}

void UGeneralSaveGame::OnUnload_Implementation()
{
	Super::OnLoad_Implementation();
}

void UGeneralSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		SaveData.CameraDistance = PlayerController->GetCameraDistance(false);
	}
}
