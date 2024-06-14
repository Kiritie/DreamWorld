// Fill out your copyright notice in the Description page of Project Settings.

#include "Vitality/States/DWVitalityState_Default.h"

UDWVitalityState_Default::UDWVitalityState_Default()
{
	
}

void UDWVitalityState_Default::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

void UDWVitalityState_Default::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWVitalityState_Default::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWVitalityState_Default::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWVitalityState_Default::OnTermination()
{
	Super::OnTermination();
}
