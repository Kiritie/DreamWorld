// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Initializing.h"

#include "TimerManager.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Starting.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Menu/WidgetMainMenu.h"

UDWProcedure_Initializing::UDWProcedure_Initializing()
{
	ProcedureName = FName("Initializing");
	ProcedureDisplayName = FText::FromString(TEXT("初始化"));
}

#if WITH_EDITOR
void UDWProcedure_Initializing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Initializing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Initializing::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Initializing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UWidgetModuleStatics::CreateUserWidget<UWidgetMainMenu>();

	UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Starting>();
}

void UDWProcedure_Initializing::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Initializing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Initializing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
