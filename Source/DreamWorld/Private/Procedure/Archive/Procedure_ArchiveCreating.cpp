// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/Procedure_ArchiveCreating.h"

#include "Camera/CameraModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Loading.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"
#include "Voxel/VoxelModule.h"
#include "Common/DWCommonTypes.h"
#include "Procedure/Archive/Procedure_ArchiveChoosing.h"

UProcedure_ArchiveCreating::UProcedure_ArchiveCreating()
{
	ProcedureName = FName("ArchiveCreating");
	ProcedureDisplayName = FText::FromString(TEXT("创建存档"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;
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

	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UProcedure_ArchiveCreating::OnPlayerChanged);
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
		if(USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex == -1)
		{
			USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			CreatePlayer(PlayerSaveData, EPhase::Lesser);
			CreateWorld(WorldSaveData, EPhase::Lesser);
			CreateArchive(USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>());
		}
		else
		{
			USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Loading>();
		}
	}
	else
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetArchiveCreatingPanel>();
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
	UCommonStatics::GetPlayerController<ADWPlayerController>()->LoadSaveData(&InPlayerSaveData, InPhase);
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().PlayerData = InPlayerSaveData;
}

void UProcedure_ArchiveCreating::CreateWorld(FDWWorldSaveData& InWorldSaveData, EPhase InPhase)
{
	UVoxelModule::Get().LoadSaveData(&InWorldSaveData, InPhase);
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().WorldData = InWorldSaveData;
}

void UProcedure_ArchiveCreating::CreateArchive(FDWArchiveSaveData& InArchiveSaveData)
{
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->SetSaveData(&InArchiveSaveData);
	UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Loading>();
}
