// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Initializing.h"

#include "TimerManager.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Starting.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Menu/WidgetMainMenu.h"

UProcedure_Initializing::UProcedure_Initializing()
{
	ProcedureName = FName("Initializing");
	ProcedureDisplayName = FText::FromString(TEXT("初始化"));
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

	UWidgetModuleStatics::CreateUserWidget<UWidgetMainMenu>();

	UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Starting>();
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
