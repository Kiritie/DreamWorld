// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Initializing.h"

#include "TimerManager.h"
#include "Camera/CameraModule.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Starting.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "Scene/SceneModule.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Loading/WidgetLoadingPanel.h"
#include "Widget/Menu/WidgetMainMenu.h"

UDWProcedure_Initializing::UDWProcedure_Initializing()
{
	ProcedureName = FName("Initializing");
	ProcedureDisplayName = FText::FromString(TEXT("初始化"));
}

#if WITH_EDITOR
void UDWProcedure_Initializing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Initializing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Initializing::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Initializing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UWidgetModuleStatics::CreateUserWidget<UWidgetMainMenu>();

	UWidgetModuleStatics::OpenUserWidget<UWidgetLoadingPanel>({ TEXT("初始化中..."), 3 });

	USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::PrimaryAndLesser);

	UVoxelModuleStatics::SetVoxelWorldMode(EVoxelWorldMode::Preview);

	AMainModule::PauseModuleByClass<USceneModule>();
	
	AMainModule::PauseModuleByClass<UCameraModule>();
}

void UDWProcedure_Initializing::OnRefresh()
{
	Super::OnRefresh();

	if(UVoxelModule::Get().GetWorldGeneratePercent() >= 1.f)
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Starting>();
	}
}

void UDWProcedure_Initializing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Initializing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UWidgetModuleStatics::CloseUserWidget<UWidgetLoadingPanel>();

	AMainModule::UnPauseModuleByClass<UCameraModule>();
}
