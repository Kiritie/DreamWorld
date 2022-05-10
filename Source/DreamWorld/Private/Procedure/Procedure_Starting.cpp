// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Starting.h"
#include "Gameplay/DWGameState.h"
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
	Super::OnEnter(InLastProcedure);
	
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		GameState->SetCurrentState(EDWGameState::Starting);
	}

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
				
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveChoosingPanel>();

	if(ADWVoxelModule* VoxelModule = AMainModule::GetModuleByClass<ADWVoxelModule>())
	{
		VoxelModule->SetWorldMode(EVoxelWorldMode::Preview);
	}

	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(GeneralSaveGame->GetCurrentArchiveID() == -1)
		{
			if(UDWArchiveSaveGame* ArchiveSaveGame = USaveGameModuleBPLibrary::HasSaveGame<UDWArchiveSaveGame>() ? USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>() : USaveGameModuleBPLibrary::CreateSaveGame<UDWArchiveSaveGame>(-1))
			{
				ArchiveSaveGame->Load();
			}
		}
		else
		{
			USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(GeneralSaveGame->GetCurrentArchiveID());
		}
	}
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
