// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Starting.h"

#include "Audio/AudioModuleBPLibrary.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Camera/Roam/RoamCameraPawn.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Initializing.h"
#include "Procedure/Procedure_Pausing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Scene/SceneModule.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/WidgetContextBox.h"

UProcedure_Starting::UProcedure_Starting()
{
	ProcedureName = FName("Starting");
	ProcedureDisplayName = FText::FromString(TEXT("开始"));

	bTrackTarget = true;
	TrackTargetMode = ETrackTargetMode::LocationOnly;
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
	
	UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UProcedure_Starting::OnPlayerChanged);
}

void UProcedure_Starting::OnEnter(UProcedureBase* InLastProcedure)
{
	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveChoosingPanel>();

	if(!InLastProcedure || InLastProcedure->IsA<UProcedure_Initializing>())
	{
		USaveGameModuleBPLibrary::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1);
	}
	else
	{
		AVoxelModule::Get()->UnloadSaveData(EPhase::Final);
		UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(EPhase::Final);
	}
	AMainModule::PauseModuleByClass<ASceneModule>();

	Super::OnEnter(InLastProcedure);
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
