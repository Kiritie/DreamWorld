// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_ArchiveCreating.h"

#include "Camera/CameraModuleBPLibrary.h"
#include "Character/CharacterModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "Event/EventModuleBPLibrary.h"
#include "FSM/Components/FSMComponent.h"
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
}

void UProcedure_ArchiveCreating::OnEnter(UProcedureBase* InLastProcedure)
{
	if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
	{
		PlayerCharacter->SetActorHiddenInGame(false);
		PlayerCharacter->GetFSMComponent()->OnStateChanged.AddDynamic(this, &UProcedure_ArchiveCreating::OnTargetCharacterStateChanged);
		OperationTarget = PlayerCharacter;
	}
	
	Super::OnEnter(InLastProcedure);

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

	if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
	{
		PlayerCharacter->GetFSMComponent()->OnStateChanged.RemoveDynamic(this, &UProcedure_ArchiveCreating::OnTargetCharacterStateChanged);
	}
}

void UProcedure_ArchiveCreating::OnTargetCharacterStateChanged(UFiniteStateBase* InFiniteState)
{
	if(InFiniteState->IsA<UDWCharacterState_Walk>())
	{
		ResetCameraView();
	}
}

void UProcedure_ArchiveCreating::CreateArchive(FDWArchiveSaveData InArchiveSaveData)
{
	USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->SetSaveData(&InArchiveSaveData);

	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
}
