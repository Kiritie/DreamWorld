// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/Procedure_ArchiveChoosing.h"

#include "Camera/CameraModuleStatics.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Archive/Procedure_ArchiveCreating.h"
#include "Procedure/Procedure_Loading.h"
#include "Procedure/Procedure_Starting.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UProcedure_ArchiveChoosing::UProcedure_ArchiveChoosing()
{
	ProcedureName = FName("ArchiveChoosing");
	ProcedureDisplayName = FText::FromString(TEXT("选择存档"));

	bTrackTarget = true;
	TrackTargetMode = ETrackTargetMode::LocationOnly;
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

	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UProcedure_ArchiveChoosing::OnPlayerChanged);
}

void UProcedure_ArchiveChoosing::OnEnter(UProcedureBase* InLastProcedure)
{
	if(InLastProcedure && InLastProcedure->IsA<UProcedure_Starting>())
	{
		CameraViewYaw = UCameraModuleStatics::GetCameraRotation().Yaw;
	}
	
	Super::OnEnter(InLastProcedure);

	UWidgetModuleStatics::OpenUserWidget<UWidgetArchiveChoosingPanel>();

	UWidgetModuleStatics::CreateUserWidget<UWidgetArchiveCreatingPanel>();
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

void UProcedure_ArchiveChoosing::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}

void UProcedure_ArchiveChoosing::CreateArchive()
{
	if(USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->IsSaved())
	{
		USaveGameModuleStatics::CreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
	}
	UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_ArchiveCreating>();
}

void UProcedure_ArchiveChoosing::RemoveArchive(int32 InArchiveID)
{
	const bool bNeedCreateArchive = USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex == InArchiveID;
	USaveGameModuleStatics::DestroySaveGame<UDWArchiveSaveGame>(InArchiveID);
	if(bNeedCreateArchive) USaveGameModuleStatics::CreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
	UWidgetModuleStatics::GetUserWidget<UWidgetArchiveChoosingPanel>()->Refresh();
}

void UProcedure_ArchiveChoosing::ChooseArchive(int32 InArchiveID)
{
	if(USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex != InArchiveID)
	{
		USaveGameModuleStatics::LoadSaveGame<UDWArchiveSaveGame>(InArchiveID, EPhase::Primary);
	}
	UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Loading>();
}
