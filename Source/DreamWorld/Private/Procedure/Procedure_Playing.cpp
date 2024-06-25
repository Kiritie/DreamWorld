// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Playing.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Main/MainModule.h"
#include "Procedure/Procedure_Loading.h"
#include "Procedure/Procedure_Starting.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Context/WidgetContextBox.h"

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

	if(!InLastProcedure || InLastProcedure->IsA<UProcedure_Loading>())
	{
		AMainModule::UnPauseModuleByClass<USceneModule>();
		
		AMainModule::UnPauseModuleByClass<UCameraModule>();

		ISceneActorInterface::Execute_SetActorVisible(UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(), true);

		if(!UWidgetModuleStatics::HasUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::CreateUserWidget<UWidgetGameHUD>(UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>());
		}

		UWidgetModuleStatics::OpenUserWidget<UWidgetGameHUD>();
	}

	UAchievementModuleStatics::UnlockAchievement(FName("FirstPlay"));
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
	
	if(InNextProcedure && InNextProcedure->IsA<UProcedure_Starting>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->ClearContext();
	}
}
