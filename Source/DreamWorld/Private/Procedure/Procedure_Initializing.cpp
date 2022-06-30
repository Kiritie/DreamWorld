// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Initializing.h"

#include "TimerManager.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"

UProcedure_Initializing::UProcedure_Initializing()
{
	ProcedureName = FName("Initializing");
	ProcedureDisplayName = FText::FromString(TEXT("Initializing"));
}

#if WITH_EDITOR
void UProcedure_Initializing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Initializing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Initializing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Initializing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetMainMenu>();
}

void UProcedure_Initializing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Initializing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Initializing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
