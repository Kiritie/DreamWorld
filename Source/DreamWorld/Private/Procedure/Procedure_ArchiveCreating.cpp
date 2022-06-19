// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_ArchiveCreating.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Voxel/EventHandle_ChangeVoxelWorldState.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Loading.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UProcedure_ArchiveCreating::UProcedure_ArchiveCreating()
{
	ProcedureName = FName("ArchiveCreating");
	ProcedureDisplayName = FText::FromString(TEXT("ArchiveCreating"));
}

#if WITH_EDITOR
void UProcedure_ArchiveCreating::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_ArchiveCreating::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_ArchiveCreating::OnInitialize()
{
	Super::OnInitialize();

	UEventModuleBPLibrary::SubscribeEvent<UEventHandle_ChangeVoxelWorldState>(this, FName("OnChangeVoxelWorldState"));
}

void UProcedure_ArchiveCreating::OnEnter(UProcedureBase* InLastProcedure)
{
	OperationTarget = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>();
	
	Super::OnEnter(InLastProcedure);
	
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>())
	{
		GameState->SetCurrentState(EDWGameState::ArchiveCreating);
	}

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveCreatingPanel>();

	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetLoadingPanel>();
}

void UProcedure_ArchiveCreating::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_ArchiveCreating::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_ArchiveCreating::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}

void UProcedure_ArchiveCreating::CreateArchive(FDWArchiveSaveData InArchiveSaveData)
{
	USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->SetSaveData(InArchiveSaveData);

	UCameraModuleBPLibrary::SwitchCameraByClass(nullptr);

	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->Possess(UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>());
	
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
}

void UProcedure_ArchiveCreating::OnChangeVoxelWorldState(UObject* InSender, UEventHandle_ChangeVoxelWorldState* InEventHandle)
{
	switch(InEventHandle->WorldState)
	{
		case EVoxelWorldState::BasicGenerated:
		{
			ResetCameraView();
			break;
		}
		default: break;
	}
}
