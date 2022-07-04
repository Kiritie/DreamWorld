// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Defend.h"

UDWCharacterState_Defend::UDWCharacterState_Defend()
{
	StateName = FName("Defend");
}

void UDWCharacterState_Defend::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Defend::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Defend::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Defend::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
