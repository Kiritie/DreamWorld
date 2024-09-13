// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Starting.h"

#include "Camera/CameraModule.h"
#include "Camera/CameraModuleStatics.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Initializing.h"
#include "Procedure/Archive/Procedure_ArchiveChoosing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Menu/WidgetMainMenu.h"

UProcedure_Starting::UProcedure_Starting()
{
	ProcedureName = FName("Starting");
	ProcedureDisplayName = FText::FromString(TEXT("开始"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;
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
	
	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UProcedure_Starting::OnPlayerChanged);
}

void UProcedure_Starting::OnEnter(UProcedureBase* InLastProcedure)
{
	if(InLastProcedure && InLastProcedure->IsA<UProcedure_ArchiveChoosing>())
	{
		CameraViewParams.CameraViewYaw = UCameraModuleStatics::GetCameraRotation().Yaw;
	}

	Super::OnEnter(InLastProcedure);

	if(!InLastProcedure || InLastProcedure->IsA<UProcedure_Initializing>())
	{
		USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::PrimaryAndLesser);
	}
	else
	{
		USaveGameModuleStatics::UnloadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Lesser);
	}

	AMainModule::PauseModuleByClass<USceneModule>();

	UWidgetModuleStatics::OpenUserWidget<UWidgetMainMenu>();
	UWidgetModuleStatics::CreateUserWidget<UWidgetArchiveChoosingPanel>();
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

void UProcedure_Starting::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}
