// Fill out your copyright notice in the Description page of Project Settings.

#include "Vitality/States/DWVitalityState_Default.h"

UDWVitalityState_Default::UDWVitalityState_Default()
{
	
}

void UDWVitalityState_Default::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWVitalityState_Default::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWVitalityState_Default::OnRefresh()
{
	Super::OnRefresh();
}

void UDWVitalityState_Default::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWVitalityState_Default::OnTermination()
{
	Super::OnTermination();
}
