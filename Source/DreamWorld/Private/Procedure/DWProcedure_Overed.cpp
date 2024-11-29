// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Overed.h"

UDWProcedure_Overed::UDWProcedure_Overed()
{
	ProcedureName = FName("Overed");
	ProcedureDisplayName = FText::FromString(TEXT("结束"));
}

#if WITH_EDITOR
void UDWProcedure_Overed::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Overed::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Overed::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Overed::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);
}

void UDWProcedure_Overed::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Overed::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Overed::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
