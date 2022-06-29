// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Starting.h"

#include "Audio/AudioModuleBPLibrary.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Camera/Roam/RoamCameraPawn.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/Procedure_Initializing.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

UProcedure_Starting::UProcedure_Starting()
{
	ProcedureName = FName("Starting");
	ProcedureDisplayName = FText::FromString(TEXT("Starting"));
	
	BGMSound = nullptr;
}

#if WITH_EDITOR
void UProcedure_Starting::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Starting::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Starting::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Starting::OnEnter(UProcedureBase* InLastProcedure)
{
	UGlobalBPLibrary::GetGameState<ADWGameState>()->SetCurrentState(EDWGameState::Starting);

	UAudioModuleBPLibrary::PlaySingleSound2D(BGMSound, FName("BGM"));

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveChoosingPanel>();

	AMainModule::GetModuleByClass<ADWVoxelModule>()->SetWorldMode(EVoxelWorldMode::Preview);

	if(!InLastProcedure || InLastProcedure->IsA(UProcedure_Initializing::StaticClass()))
	{
		if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCurrentArchiveID() == -1)
		{
			if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::GetOrCreateSaveGame<UDWArchiveSaveGame>(-1))
			{
				ArchiveSaveGame->Load();
			}
		}
		else
		{
			USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCurrentArchiveID());
		}
		if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
		{
			PlayerCharacter->SetActorHiddenInGame(true);
		}
	}
	else
	{
		if(AVoxelModule* VoxelModule = AMainModule::GetModuleByClass<AVoxelModule>())
		{
			USaveGameModuleBPLibrary::UnloadDataObject(VoxelModule);
		}
		if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
		{
			PlayerCharacter->SetActorHiddenInGame(true);
		}
	}

	Super::OnEnter(InLastProcedure);
}

void UProcedure_Starting::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Starting::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Starting::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
