// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/DWGeneralSaveGame.h"

#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	
	if(ADWPlayerController* PlayerController = UGlobalBPLibrary::GetPlayerController<ADWPlayerController>(GWorld))
	{
		SaveData.CameraDistance = PlayerController->GetCurrentCameraDistance();
	}
}
