// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Preparing.h"
#include "Gameplay/DWGameState.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

UProcedure_Preparing::UProcedure_Preparing()
{
	ProcedureName = FName("Preparing");
	ProcedureDisplayName = FText::FromString(TEXT("Preparing"));
}

#if WITH_EDITOR
void UProcedure_Preparing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Preparing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Preparing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Preparing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);
	
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		GameState->SetCurrentState(EDWGameState::Preparing);
	}

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetArchiveChoosingPanel>();

	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveCreatingPanel>();
}

void UProcedure_Preparing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Preparing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Preparing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
