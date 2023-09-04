// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Playing.h"

#include "AchievementSubSystem.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Camera/CameraModule.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Character/CharacterModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/WHGameInstance.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/Procedure_Loading.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "Scene/SceneModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Procedure/Procedure_Starting.h"
#include "Widget/WidgetContextBox.h"
#include "Widget/WidgetGeneratePanel.h"
#include "Widget/WidgetShopPanel.h"

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

		if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerPawn<ADWPlayerCharacter>())
		{
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, true);
			
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGeneratePanel>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetShopPanel>(PlayerCharacter);
		}
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGameHUD>();
	}

	UGlobalBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstPlay"));
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
