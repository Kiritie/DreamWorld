// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Starting.h"

#include "Camera/CameraModule.h"
#include "Camera/CameraModuleStatics.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Initializing.h"
#include "Procedure/Archive/DWProcedure_ArchiveChoosing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Scene/SceneModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Menu/WidgetMainMenu.h"

UDWProcedure_Starting::UDWProcedure_Starting()
{
	ProcedureName = FName("Starting");
	ProcedureDisplayName = FText::FromString(TEXT("开始"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;
}

#if WITH_EDITOR
void UDWProcedure_Starting::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Starting::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Starting::OnInitialize()
{
	Super::OnInitialize();
	
	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UDWProcedure_Starting::OnPlayerChanged);
}

void UDWProcedure_Starting::OnEnter(UProcedureBase* InLastProcedure)
{
	if(InLastProcedure && InLastProcedure->IsA<UDWProcedure_ArchiveChoosing>())
	{
		CameraViewParams.CameraViewYaw = UCameraModuleStatics::GetCameraRotation().Yaw;
	}

	Super::OnEnter(InLastProcedure);

	if(!InLastProcedure || InLastProcedure->IsA<UDWProcedure_Initializing>())
	{
		USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::PrimaryAndLesser);
	}
	else
	{
		USaveGameModuleStatics::UnloadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Lesser);
	}
	
	UVoxelModuleStatics::SetVoxelWorldMode(EVoxelWorldMode::Preview);

	AMainModule::PauseModuleByClass<USceneModule>();

	UWidgetModuleStatics::OpenUserWidget<UWidgetMainMenu>();
	UWidgetModuleStatics::CreateUserWidget<UWidgetArchiveChoosingPanel>();
}

void UDWProcedure_Starting::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Starting::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Starting::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}

void UDWProcedure_Starting::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}
