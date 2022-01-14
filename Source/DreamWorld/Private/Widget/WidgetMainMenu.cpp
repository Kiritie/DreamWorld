// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetMainMenu.h"

#include "Main/MainModule.h"
#include "Module/DWSaveGameModule.h"
#include "SaveGame/ArchiveSaveGame.h"
#include "SaveGame/ArchiveSaveGameData.h"
#include "SaveGame/GeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

UWidgetMainMenu::UWidgetMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("MainMenu");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::UIOnly;
}

void UWidgetMainMenu::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetMainMenu::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);

	if(UGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UGeneralSaveGame>())
	{
		if(GeneralSaveGame->GeneralData.CurrentArchiveID == -1)
		{
			if(UArchiveSaveGameData* ArchiveSaveGameData = NewObject<UArchiveSaveGameData>(this))
			{
				USaveGameModuleBPLibrary::CreateSaveGame<UArchiveSaveGame>(ArchiveSaveGameData);
			}
		}
		else
		{
			USaveGameModuleBPLibrary::LoadSaveGame<UArchiveSaveGame>(GeneralSaveGame->GeneralData.CurrentArchiveID);
		}
	}
}

void UWidgetMainMenu::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}
