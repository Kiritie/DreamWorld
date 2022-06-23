// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Playing.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/Procedure_Loading.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"

UProcedure_Playing::UProcedure_Playing()
{
	ProcedureName = FName("Playing");
	ProcedureDisplayName = FText::FromString(TEXT("Playing"));
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
		
	UGlobalBPLibrary::GetGameState<ADWGameState>()->SetCurrentState(EDWGameState::Playing);

	if(InLastProcedure->IsA(UProcedure_Loading::StaticClass()))
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGameHUD>();
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryBar>();
		
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->Initialize(UGlobalBPLibrary::GetPlayerCharacter());
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->Initialize(UGlobalBPLibrary::GetPlayerCharacter());
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->Initialize(UGlobalBPLibrary::GetPlayerCharacter());
	}
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
}
