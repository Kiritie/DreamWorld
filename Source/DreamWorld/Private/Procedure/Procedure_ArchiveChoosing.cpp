// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_ArchiveChoosing.h"

#include "Character/CharacterModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_ArchiveCreating.h"
#include "Procedure/Procedure_Loading.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"
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
	
	if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
	{
		PlayerCharacter->SetActorHiddenInGame(true);
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
	if(USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->IsSaved())
	{
		USaveGameModuleBPLibrary::CreateSaveGame<UDWArchiveSaveGame>(-1, true);
	}
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_ArchiveCreating>();
}

void UProcedure_ArchiveChoosing::RemoveArchive(int32 InArchiveID)
{
	const bool bNeedCreateArchive = USaveGameModuleBPLibrary::GetActiveSaveIndex<UDWArchiveSaveGame>() == InArchiveID;
	USaveGameModuleBPLibrary::DestroySaveGame<UDWArchiveSaveGame>(InArchiveID);
	if(bNeedCreateArchive) USaveGameModuleBPLibrary::CreateSaveGame<UDWArchiveSaveGame>(-1, true);
	UWidgetModuleBPLibrary::GetUserWidget<UWidgetArchiveChoosingPanel>()->Refresh();
}

void UProcedure_ArchiveChoosing::ChooseArchive(int32 InArchiveID)
{
	if(USaveGameModuleBPLibrary::GetActiveSaveIndex<UDWArchiveSaveGame>() != InArchiveID)
	{
		USaveGameModuleBPLibrary::SetActiveSaveIndex<UDWArchiveSaveGame>(InArchiveID);
	}
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
}
