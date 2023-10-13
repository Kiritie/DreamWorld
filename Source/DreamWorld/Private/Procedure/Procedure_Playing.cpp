// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Playing.h"

#include "Achievement/AchievementModuleBPLibrary.h"
#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/WHGameInstance.h"
#include "Main/MainModule.h"
#include "Procedure/Procedure_Loading.h"
#include "Procedure/Procedure_Starting.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetContextBox.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"

UProcedure_Playing::UProcedure_Playing()
{
	ProcedureName = FName("Playing");
	ProcedureDisplayName = FText::FromString(TEXT("游玩"));
}

#if WITH_EDITOR
void UProcedure_Playing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Playing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Playing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Playing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(InLastProcedure->IsA<UProcedure_Loading>())
	{
		AMainModule::UnPauseModuleByClass<ASceneModule>();
		AMainModule::UnPauseModuleByClass<ACameraModule>();

		ISceneActorInterface::Execute_SetActorVisible(UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>(), true);
		
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGameHUD>();
	}

	UAchievementModuleBPLibrary::UnlockAchievement(FName("FirstPlay"));
}

void UProcedure_Playing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Playing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Playing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
	if(InNextProcedure->IsA<UProcedure_Starting>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetContextBox>()->ClearContext();
	}
}
