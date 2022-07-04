// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Revive.h"

UDWCharacterState_Revive::UDWCharacterState_Revive()
{
	StateName = FName("Revive");
}

void UDWCharacterState_Revive::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Revive::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Revive::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Revive::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Revive::OnTermination()
{
	Super::OnTermination();
}
