// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Loading.h"

#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Playing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Scene/SceneModule.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Loading/WidgetLoadingPanel.h"

UDWProcedure_Loading::UDWProcedure_Loading()
{
	ProcedureName = FName("Loading");
	ProcedureDisplayName = FText::FromString(TEXT("加载"));
}

#if WITH_EDITOR
void UDWProcedure_Loading::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Loading::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Loading::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Loading::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	AMainModule::PauseModuleByClass<USceneModule>();
	AMainModule::PauseModuleByClass<UCameraModule>();

	UWidgetModuleStatics::OpenUserWidget<UWidgetLoadingPanel>({ FString(TEXT("生成世界中...")) });

	UWidgetGameHUD* GameHUD = UWidgetModuleStatics::CreateUserWidget<UWidgetGameHUD>();

	USaveGameModuleStatics::LoadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Final);
	
	UVoxelModuleStatics::SetVoxelWorldMode(EVoxelWorldMode::Default);

	GameHUD->Init(UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(), nullptr, true);
}

void UDWProcedure_Loading::OnRefresh()
{
	Super::OnRefresh();

	if(UVoxelModule::Get().IsWorldBasicGenerated())
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Playing>();
	}
}

void UDWProcedure_Loading::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Loading::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UWidgetModuleStatics::CloseUserWidget<UWidgetLoadingPanel>();
}
