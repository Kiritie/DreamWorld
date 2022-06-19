// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_ArchiveChoosing.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_ArchiveCreating.h"
#include "Procedure/Procedure_Loading.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UProcedure_ArchiveChoosing::UProcedure_ArchiveChoosing()
{
	ProcedureName = FName("ArchiveChoosing");
	ProcedureDisplayName = FText::FromString(TEXT("ArchiveChoosing"));
}

#if WITH_EDITOR
void UProcedure_ArchiveChoosing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_ArchiveChoosing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_ArchiveChoosing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_ArchiveChoosing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);
	
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>())
	{
		GameState->SetCurrentState(EDWGameState::ArchiveChoosing);
	}

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveChoosingPanel>();

	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveCreatingPanel>();
}

void UProcedure_ArchiveChoosing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_ArchiveChoosing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_ArchiveChoosing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}

void UProcedure_ArchiveChoosing::CreateArchive()
{
	if(!USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->IsPreview())
	{
		USaveGameModuleBPLibrary::UnloadSaveGame<UDWArchiveSaveGame>();
		USaveGameModuleBPLibrary::CreateSaveGame<UDWArchiveSaveGame>(-1, true);
	}
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_ArchiveCreating>();
}

void UProcedure_ArchiveChoosing::RemoveArchive(int32 InArchiveID)
{
	USaveGameModuleBPLibrary::DestroySaveGame<UDWArchiveSaveGame>(InArchiveID);
	if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCurrentArchiveID() != InArchiveID)
	{
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetArchiveBasicDatas().Remove(InArchiveID);
		USaveGameModuleBPLibrary::CreateSaveGame<UDWArchiveSaveGame>(-1, true);
	}
	UWidgetModuleBPLibrary::GetUserWidget<UWidgetArchiveChoosingPanel>()->Refresh();
}

void UProcedure_ArchiveChoosing::ChooseArchive(int32 InArchiveID)
{
	if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCurrentArchiveID() != InArchiveID)
	{
		USaveGameModuleBPLibrary::UnloadSaveGame<UDWArchiveSaveGame>();
		USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->SetCurrentArchiveID(InArchiveID);
	}
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
}
