// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Loading.h"

#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Loading/WidgetLoadingPanel.h"

UProcedure_Loading::UProcedure_Loading()
{
	ProcedureName = FName("Loading");
	ProcedureDisplayName = FText::FromString(TEXT("加载"));
}

#if WITH_EDITOR
void UProcedure_Loading::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Loading::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Loading::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Loading::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	AMainModule::PauseModuleByClass<UCameraModule>();

	UWidgetModuleStatics::OpenUserWidget<UWidgetLoadingPanel>({ FString(TEXT("生成世界中...")) });

	UWidgetGameHUD* GameHUD = UWidgetModuleStatics::CreateUserWidget<UWidgetGameHUD>();

	USaveGameModuleStatics::LoadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Final);

	GameHUD->Init(UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>());
}

void UProcedure_Loading::OnRefresh()
{
	Super::OnRefresh();

	if(UVoxelModule::Get().IsBasicGenerated())
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Playing>();
	}
}

void UProcedure_Loading::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Loading::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UWidgetModuleStatics::CloseUserWidget<UWidgetLoadingPanel>();
}
