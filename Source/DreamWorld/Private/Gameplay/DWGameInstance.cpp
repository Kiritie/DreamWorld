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
#include "Widget/WidgetRoleChoosingPanel.h"
#include "Widget/WidgetRoleCreatingPanel.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/WidgetWorldChoosingPanel.h"
#include "Widget/WidgetWorldCreatingPanel.h"
#include "Widget/WidgetPrimaryPanel.h"
#include "Widget/Inventory/WidgetInventoryBox.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Gameplay/DWGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGameGeneral.h"
#include "SaveGame/SaveGameArchive.h"
#include "World/WorldManager.h"
#include "Gameplay/DWGameState.h"

UDWGameInstance::UDWGameInstance()
{
	// set default pawn class to our Blueprinted character

	GeneralDataSave = nullptr;
	ArchiveDataSaves = TMap<FString, USaveGameArchive*>();

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

void UDWGameInstance::Initialize()
{
	ResetGeneralData();
	LoadGeneralData();
}

void UDWGameInstance::ResetGeneralData()
{
	GeneralDataSave = nullptr;
	ArchiveDataSaves.Empty();
}

void UDWGameInstance::SaveGeneralData(bool bRefresh)
{
	if(GeneralDataSave)
	{
		if(bRefresh) GeneralDataSave->RefreshSaveData();
		UGameplayStatics::SaveGameToSlot(GeneralDataSave, TEXT("GeneralData"), UserIndex);
	}
}

USaveGameGeneral* UDWGameInstance::LoadGeneralData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GeneralData"), UserIndex))
	{
		GeneralDataSave = Cast<USaveGameGeneral>(UGameplayStatics::LoadGameFromSlot(TEXT("GeneralData"), UserIndex));
	}
	else
	{
		CreateGeneralData(FGeneralSaveData());
	}

	if(GeneralDataSave->GetCurrentArchiveName().IsEmpty())
	{
		FArchiveSaveData ArchiveData;
		ArchiveData.Name = TEXT("Preview");
		ArchiveData.PlayerData.ID = FName("Player_1");
		CreateArchiveData(ArchiveData);
	}

	return GeneralDataSave;
}

void UDWGameInstance::CreateGeneralData(FGeneralSaveData InGeneralData, bool bSaveData)
{
	GeneralDataSave = Cast<USaveGameGeneral>(UGameplayStatics::CreateSaveGameObject(USaveGameGeneral::StaticClass()));
	if(GeneralDataSave)
	{
		GeneralDataSave->SetGeneralData(InGeneralData);
		if(bSaveData) SaveGeneralData(false);
	}
}

void UDWGameInstance::DeleteGeneralData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GeneralData"), UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("GeneralData"), UserIndex);
	}
	GeneralDataSave = nullptr;
}

FGeneralSaveData UDWGameInstance::GetGeneralData() const
{
	if(GeneralDataSave)
	{
		return GeneralDataSave->GetGeneralData();
	}
	return FGeneralSaveData();
}

bool UDWGameInstance::IsExistArchiveData(const FString& InArchiveName)
{
	return GetArchiveDatas().Contains(InArchiveName);
}

void UDWGameInstance::SaveArchiveData(const FString& InArchiveName, bool bRefresh)
{
	if(USaveGameArchive* ArchiveDataSave = LoadArchiveData(InArchiveName))
	{
		if(bRefresh) ArchiveDataSave->RefreshSaveData();
		
		WHDebug(FString::Printf(TEXT("Saving world : %s, %dchunks."), *InArchiveName, ArchiveDataSave->GetChunkDatas().Num()), FColor::Cyan);

		UGameplayStatics::SaveGameToSlot(ArchiveDataSave, TEXT("ArchiveData_") + InArchiveName, UserIndex);
	}
}

USaveGameArchive* UDWGameInstance::LoadArchiveData(FString InArchiveName)
{
	if(GeneralDataSave)
	{
		if(InArchiveName.IsEmpty())
		{
			InArchiveName = GeneralDataSave->GetCurrentArchiveName();
		}
		else
		{
			GeneralDataSave->SetCurrentArchiveName(InArchiveName);
		}
	}
	
	if(ArchiveDataSaves.Contains(InArchiveName)) return ArchiveDataSaves[InArchiveName];
	
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ArchiveData_") + InArchiveName, UserIndex))
	{
		if(USaveGameArchive* ArchiveDataSave = Cast<USaveGameArchive>(UGameplayStatics::LoadGameFromSlot(TEXT("ArchiveData_") + InArchiveName, UserIndex)))
		{
			ArchiveDataSaves.Add(InArchiveName, ArchiveDataSave);
			WHDebug(FString::Printf(TEXT("Loading world : %s, %dchunks."), *InArchiveName, ArchiveDataSave->GetChunkDatas().Num()), FColor::Cyan);
			return ArchiveDataSave;
		}
	}
	return nullptr;
}

void UDWGameInstance::UnloadArchiveData(const FString& InArchiveName, bool bSaveData /*= true*/)
{
	if(!ArchiveDataSaves.Contains(InArchiveName)) return;
	
	if(bSaveData) SaveArchiveData(InArchiveName);
	ArchiveDataSaves[InArchiveName]->ConditionalBeginDestroy();
	ArchiveDataSaves.Remove(InArchiveName);
}

void UDWGameInstance::CreateArchiveData(FArchiveSaveData InArchiveData, bool bSaveData)
{
	if (IsExistArchiveData(InArchiveData.Name)) return;
	
	if(USaveGameArchive* ArchiveDataSave = Cast<USaveGameArchive>(UGameplayStatics::CreateSaveGameObject(USaveGameArchive::StaticClass())))
	{
		if(GeneralDataSave) GeneralDataSave->SetCurrentArchiveName(InArchiveData.Name);
		InArchiveData.WorldData.ArchiveName = InArchiveData.Name;
		ArchiveDataSave->SetArchiveData(InArchiveData);
		ArchiveDataSaves.Add(InArchiveData.Name, ArchiveDataSave);
		if(GeneralDataSave)
		{
			GeneralDataSave->GetArchiveDatas().Add(InArchiveData.Name, InArchiveData);
		}
		if(bSaveData)
		{
			SaveArchiveData(InArchiveData.Name, false);
			SaveGeneralData(false);
		}
	}
}

void UDWGameInstance::RemoveArchiveData(const FString& InArchiveName)
{
	if (!IsExistArchiveData(InArchiveName)) return;

	if(GeneralDataSave)
	{
		UnloadArchiveData(InArchiveName, false);
		UGameplayStatics::DeleteGameInSlot(TEXT("ArchiveData_") + InArchiveName, UserIndex);
		GeneralDataSave->GetArchiveDatas().Remove(InArchiveName);
		SaveGeneralData(false);
	}
}

FString UDWGameInstance::GetCurrentArchiveName() const
{
	if(GeneralDataSave)
	{
		return GeneralDataSave->GetCurrentArchiveName();
	}
	return TEXT("");
}

TMap<FString, FArchiveSaveData> UDWGameInstance::GetArchiveDatas() const
{
	if(GeneralDataSave)
	{
		return GeneralDataSave->GetArchiveDatas();
	}
	return TMap<FString, FArchiveSaveData>();
}
