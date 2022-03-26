// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/DWGeneralSaveGame.h"

#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "World/VoxelModule.h"

UDWGeneralSaveGame::UDWGeneralSaveGame()
{
	SaveName = FName("General");
}

void UDWGeneralSaveGame::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();
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
	
	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		SaveData.CameraDistance = PlayerController->GetCameraDistance(false);
	}
}
