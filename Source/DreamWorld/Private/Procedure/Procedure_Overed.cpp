// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Overed.h"

#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"

UProcedure_Overed::UProcedure_Overed()
{
	ProcedureName = FName("Overed");
	ProcedureDisplayName = FText::FromString(TEXT("结束"));
}

#if WITH_EDITOR
void UProcedure_Overed::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Overed::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Overed::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Overed::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);
}

void UProcedure_Overed::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Overed::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Overed::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
