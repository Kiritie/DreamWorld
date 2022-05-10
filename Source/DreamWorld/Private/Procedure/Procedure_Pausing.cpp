// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Pausing.h"

#include "Gameplay/DWGameState.h"
#include "Procedure/Procedure_Starting.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/Inventory/WidgetInventoryBar.h"

UProcedure_Pausing::UProcedure_Pausing()
{
	ProcedureName = FName("Pausing");
	ProcedureDisplayName = FText::FromString(TEXT("Pausing"));
}

#if WITH_EDITOR
void UProcedure_Pausing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Pausing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Pausing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Pausing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		GameState->SetCurrentState(EDWGameState::Pausing);
	}

	UGameplayStatics::SetGamePaused(this, true);
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->PauseModules();
	}

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetPausingMenu>();
}

void UProcedure_Pausing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Pausing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Pausing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UGameplayStatics::SetGamePaused(this, false);
	if(AMainModule* MainModule = AMainModule::Get())
	{
		MainModule->UnPauseModules();
	}

	if(InNextProcedure->IsA(UProcedure_Starting::StaticClass()))
	{
		UWidgetModuleBPLibrary::CloseUserWidget<UWidgetGameHUD>();
		UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryBar>();
	}

	UWidgetModuleBPLibrary::CloseUserWidget<UWidgetPausingMenu>();
}
