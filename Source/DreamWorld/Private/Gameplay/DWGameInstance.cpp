// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Gameplay/DWGameInstance.h"

#include "Debug/DebugModuleTypes.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWPlayerController.h"
#include "Engine/World.h"
#include "Widget/User/UserWidgetBase.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetSettingPanel.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"
#include "Widget/WidgetPrimaryPanel.h"
#include "Widget/Inventory/WidgetInventoryBox.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Gameplay/DWGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/ArchiveSaveGame.h"
#include "World/WorldManager.h"
#include "Gameplay/DWGameState.h"

UDWGameInstance::UDWGameInstance()
{
	// set default pawn class to our Blueprinted character

	GeneralSaveGame = nullptr;
	ArchiveSaveGame = nullptr;

	UserIndex = 0;
}

void UDWGameInstance::Init()
{
	Super::Init();

	UDWHelper::LoadItemDatas();
}

void UDWGameInstance::Shutdown()
{
	Super::Shutdown();

}

void UDWGameInstance::InitializeData()
{
	LoadGeneralData();
}

void UDWGameInstance::CreateGeneralData(FGeneralSaveData InGeneralData, bool bSaveData)
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GeneralData"), UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("GeneralData"), UserIndex);
	}

	GeneralSaveGame = Cast<UGeneralSaveGame>(UGameplayStatics::CreateSaveGameObject(UGeneralSaveGame::StaticClass()));
	if(GeneralSaveGame)
	{
		GeneralSaveGame->SetGeneralData(InGeneralData);
		if(bSaveData) SaveGeneralData(false);
	}
	FArchiveSaveData ArchiveData;
	ArchiveData.ID = TEXT("Preview");

	FWorldSaveData WorldData = FWorldSaveData(WorldBasicData);

	FPlayerSaveData PlayerData = FPlayerSaveData(PlayerBasicData);
	PlayerData.ID = FName("Player_1");

	ArchiveData.WorldData = WorldData;
	ArchiveData.PlayerData = PlayerData;
	CreateArchiveData(ArchiveData);
}

void UDWGameInstance::ResetGeneralData()
{
	if(!GeneralSaveGame) return;

	if (UGameplayStatics::DoesSaveGameExist(TEXT("GeneralData"), UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("GeneralData"), UserIndex);
	}

	CreateGeneralData(FGeneralSaveData());
}

void UDWGameInstance::SaveGeneralData(bool bRefresh)
{
	if(!GeneralSaveGame) return;

	if(bRefresh) GeneralSaveGame->RefreshSaveData();
	UGameplayStatics::SaveGameToSlot(GeneralSaveGame, TEXT("GeneralData"), UserIndex);
}

UGeneralSaveGame* UDWGameInstance::LoadGeneralData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GeneralData"), UserIndex))
	{
		GeneralSaveGame = Cast<UGeneralSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GeneralData"), UserIndex));
	}
	else
	{
		CreateGeneralData(FGeneralSaveData());
	}

	return GeneralSaveGame;
}

FGeneralSaveData UDWGameInstance::GetGeneralData() const
{
	if (!GeneralSaveGame) return FGeneralSaveData();

	return GeneralSaveGame->GetGeneralData();
}

void UDWGameInstance::CreateArchiveData(FArchiveSaveData InArchiveData)
{
	if (!GeneralSaveGame || IsExistArchiveData(InArchiveData.ID)) return;

	ArchiveSaveGame = Cast<UArchiveSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchiveSaveGame::StaticClass()));
	if(ArchiveSaveGame)
	{
		InArchiveData.Initialize();
		ArchiveSaveGame->SetArchiveData(InArchiveData);
		GeneralSaveGame->SetCurrentArchiveID(InArchiveData.ID);
		GeneralSaveGame->GetArchiveDatas().Add(InArchiveData.ID, InArchiveData);
		SaveArchiveData(InArchiveData.ID, false);
		SaveGeneralData(false);
	}
}

void UDWGameInstance::RemoveArchiveData(FName InArchiveID)
{
	if (!GeneralSaveGame || IsExistArchiveData(InArchiveID)) return;

	if(UGameplayStatics::DoesSaveGameExist(TEXT("ArchiveData_") + InArchiveID.ToString(), UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ArchiveData_") + InArchiveID.ToString(), UserIndex);
	}
	GeneralSaveGame->GetArchiveDatas().Remove(InArchiveID);
	SaveGeneralData(false);
}

void UDWGameInstance::SaveArchiveData(FName InArchiveID, bool bRefresh)
{
	if(UArchiveSaveGame* SaveGameArchive = LoadArchiveData(InArchiveID))
	{
		if(bRefresh)
		{
			SaveGameArchive->RefreshSaveData();
			GetArchiveDatas()[InArchiveID] = SaveGameArchive->GetArchiveData();
		}
		
		WHDebug(FString::Printf(TEXT("Saving world : %s, %dchunks."), *InArchiveID.ToString(), SaveGameArchive->GetChunkDatas().Num()), FColor::Cyan);

		UGameplayStatics::SaveGameToSlot(SaveGameArchive, TEXT("ArchiveData_") + InArchiveID.ToString(), UserIndex);
	}
}

bool UDWGameInstance::IsExistArchiveData(FName InArchiveID)
{
	return GetArchiveDatas().Contains(InArchiveID);
}

UArchiveSaveGame* UDWGameInstance::LoadArchiveData(FName InArchiveID)
{
	if(!GeneralSaveGame) return nullptr;
	
	if(InArchiveID.IsNone()) InArchiveID = GeneralSaveGame->GetCurrentArchiveID();
	
	if(!IsExistArchiveData(InArchiveID)) return nullptr;
	
	if(ArchiveSaveGame && ArchiveSaveGame->GetArchiveData().ID == InArchiveID)
	{
		return ArchiveSaveGame;
	}
	else if (UGameplayStatics::DoesSaveGameExist(TEXT("ArchiveData_") + InArchiveID.ToString(), UserIndex))
	{
		ArchiveSaveGame = Cast<UArchiveSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ArchiveData_") + InArchiveID.ToString(), UserIndex));
		if(ArchiveSaveGame)
		{
			GeneralSaveGame->SetCurrentArchiveID(InArchiveID);
			WHDebug(FString::Printf(TEXT("Loading world : %s, %dchunks."), *InArchiveID.ToString(), ArchiveSaveGame->GetChunkDatas().Num()), FColor::Cyan);
			return ArchiveSaveGame;
		}
	}
	return nullptr;
}

FName UDWGameInstance::GetCurrentArchiveID() const
{
	if(!GeneralSaveGame) return TEXT("");

	return GeneralSaveGame->GetCurrentArchiveID();
}

TMap<FName, FArchiveSaveData>& UDWGameInstance::GetArchiveDatas() const
{
	static TMap<FName, FArchiveSaveData> ArchiveDatas;
	if(GeneralSaveGame)
	{
		ArchiveDatas = GeneralSaveGame->GetArchiveDatas();
	}
	return ArchiveDatas;
}
