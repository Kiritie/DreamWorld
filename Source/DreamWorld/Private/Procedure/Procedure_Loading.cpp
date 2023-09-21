// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Loading.h"

#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonBPLibrary.h"
#include "Main/MainModule.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetModuleBPLibrary.h"

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

	AMainModule::PauseModuleByClass<ACameraModule>();

	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetGameHUD>(UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>());

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>({ FString(TEXT("生成世界中...")) });

	USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Final);
}

void UProcedure_Loading::OnRefresh()
{
	Super::OnRefresh();

	if(AVoxelModule::Get()->IsBasicGenerated())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
	}
}

void UProcedure_Loading::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Loading::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UWidgetModuleBPLibrary::CloseUserWidget<UWidgetLoadingPanel>();
}
