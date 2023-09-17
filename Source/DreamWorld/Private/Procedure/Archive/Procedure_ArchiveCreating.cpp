// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/Procedure_ArchiveCreating.h"

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
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"
#include "Voxel/VoxelModule.h"
#include "Global/DWGlobalTypes.h"
#include "Procedure/Archive/Procedure_ArchiveChoosing.h"

UProcedure_ArchiveCreating::UProcedure_ArchiveCreating()
{
	ProcedureName = FName("ArchiveCreating");
	ProcedureDisplayName = FText::FromString(TEXT("创建存档"));

	bTrackTarget = true;
	TrackTargetMode = ETrackTargetMode::LocationOnly;
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

	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UProcedure_ArchiveCreating::OnPlayerChanged);
}

void UProcedure_ArchiveCreating::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(OperationTarget && OperationTarget->Implements<USceneActorInterface>())
	{
		ISceneActorInterface::Execute_SetActorVisible(OperationTarget, true);
	}

	if(!InLastProcedure)
	{
		if(USaveGameModuleBPLibrary::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex == -1)
		{
			USaveGameModuleBPLibrary::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			CreatePlayer(PlayerSaveData, EPhase::Lesser);
			CreateWorld(WorldSaveData, EPhase::Lesser);
			CreateArchive(USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>());
		}
		else
		{
			USaveGameModuleBPLibrary::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
		}
	}
	else
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveCreatingPanel>();
	}
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

	if(InNextProcedure->IsA<UProcedure_ArchiveChoosing>())
	{
		if(OperationTarget && OperationTarget->Implements<USceneActorInterface>())
		{
			ISceneActorInterface::Execute_SetActorVisible(OperationTarget, false);
		}
	}
}

void UProcedure_ArchiveCreating::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}

void UProcedure_ArchiveCreating::CreatePlayer(FDWPlayerSaveData& InPlayerSaveData, EPhase InPhase)
{
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->LoadSaveData(&InPlayerSaveData, InPhase);
	USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().PlayerData = InPlayerSaveData;
}

void UProcedure_ArchiveCreating::CreateWorld(FDWWorldSaveData& InWorldSaveData, EPhase InPhase)
{
	AVoxelModule::Get()->LoadSaveData(&InWorldSaveData, InPhase);
	USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().WorldData = InWorldSaveData;
}

void UProcedure_ArchiveCreating::CreateArchive(FDWArchiveSaveData& InArchiveSaveData)
{
	USaveGameModuleBPLibrary::GetSaveGame<UDWArchiveSaveGame>()->SetSaveData(&InArchiveSaveData);
	UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Loading>();
}
