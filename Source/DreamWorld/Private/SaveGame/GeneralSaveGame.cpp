// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/GeneralSaveGame.h"

#include "Character/Player/DWPlayerCharacterCameraManager.h"
#include "Gameplay/DWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"

UGeneralSaveGame::UGeneralSaveGame()
{
	// set default pawn class to our Blueprinted character
}

void UGeneralSaveGame::OnCreate_Implementation(USaveGameDataBase* InSaveGameData)
{
	Super::OnCreate_Implementation(InSaveGameData);
}

void UGeneralSaveGame::OnLoad_Implementation()
{
	Super::OnLoad_Implementation();
}

void UGeneralSaveGame::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
	
	if(ADWPlayerController* PlayerController = UDWHelper::GetPlayerController(this))
	{
		GeneralData.CameraDistance = PlayerController->GetCameraManager()->GetCameraDistance();
	}
}
