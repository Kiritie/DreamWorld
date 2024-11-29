// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Playing.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/DWProcedure_Loading.h"
#include "Procedure/DWProcedure_Starting.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Context/WidgetContextBox.h"

UDWProcedure_Playing::UDWProcedure_Playing()
{
	ProcedureName = FName("Playing");
	ProcedureDisplayName = FText::FromString(TEXT("游玩"));
}

#if WITH_EDITOR
void UDWProcedure_Playing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Playing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Playing::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Playing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(!InLastProcedure || InLastProcedure->IsA<UDWProcedure_Loading>())
	{
		AMainModule::UnPauseModuleByClass<USceneModule>();
		
		AMainModule::UnPauseModuleByClass<UCameraModule>();

		ISceneActorInterface::Execute_SetActorVisible(UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(), true);

		UWidgetModuleStatics::OpenUserWidget<UWidgetGameHUD>();
	}

	UAchievementModuleStatics::UnlockAchievement(FName("FirstPlay"));
}

void UDWProcedure_Playing::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Playing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Playing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
	
	if(InNextProcedure && InNextProcedure->IsA<UDWProcedure_Starting>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->ClearContext();
	}
}
