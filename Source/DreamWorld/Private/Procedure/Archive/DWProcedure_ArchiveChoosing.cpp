// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/DWProcedure_ArchiveChoosing.h"

#include "Camera/CameraModuleStatics.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Archive/DWProcedure_ArchiveCreating.h"
#include "Procedure/DWProcedure_Loading.h"
#include "Procedure/DWProcedure_Starting.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UDWProcedure_ArchiveChoosing::UDWProcedure_ArchiveChoosing()
{
	ProcedureName = FName("ArchiveChoosing");
	ProcedureDisplayName = FText::FromString(TEXT("选择存档"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;
}

#if WITH_EDITOR
void UDWProcedure_ArchiveChoosing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_ArchiveChoosing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_ArchiveChoosing::OnInitialize()
{
	Super::OnInitialize();

	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UDWProcedure_ArchiveChoosing::OnPlayerChanged);
}

void UDWProcedure_ArchiveChoosing::OnEnter(UProcedureBase* InLastProcedure)
{
	if(InLastProcedure && InLastProcedure->IsA<UDWProcedure_Starting>())
	{
		CameraViewParams.CameraViewYaw = UCameraModuleStatics::GetCameraRotation().Yaw;
	}
	
	Super::OnEnter(InLastProcedure);

	UWidgetModuleStatics::OpenUserWidget<UWidgetArchiveChoosingPanel>();

	UWidgetModuleStatics::CreateUserWidget<UWidgetArchiveCreatingPanel>();
}

void UDWProcedure_ArchiveChoosing::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_ArchiveChoosing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_ArchiveChoosing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}

void UDWProcedure_ArchiveChoosing::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}

void UDWProcedure_ArchiveChoosing::CreateArchive()
{
	if(USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->IsSaved())
	{
		USaveGameModuleStatics::CreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::PrimaryAndLesser);
	}
	UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_ArchiveCreating>();
}

void UDWProcedure_ArchiveChoosing::RemoveArchive(int32 InArchiveID)
{
	const bool bNeedCreateArchive = USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex == InArchiveID;
	USaveGameModuleStatics::DestroySaveGame<UDWArchiveSaveGame>(InArchiveID);
	if(bNeedCreateArchive) USaveGameModuleStatics::CreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::PrimaryAndLesser);
	UWidgetModuleStatics::GetUserWidget<UWidgetArchiveChoosingPanel>()->Refresh();
}

void UDWProcedure_ArchiveChoosing::ChooseArchive(int32 InArchiveID)
{
	if(USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex != InArchiveID)
	{
		USaveGameModuleStatics::LoadSaveGame<UDWArchiveSaveGame>(InArchiveID, EPhase::PrimaryAndLesser);
	}
	UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Loading>();
}
