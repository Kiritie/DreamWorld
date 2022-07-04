// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Interrup.h"

UDWCharacterState_Interrup::UDWCharacterState_Interrup()
{
	StateName = FName("Interrup");
}

void UDWCharacterState_Interrup::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Interrup::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Interrup::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Interrup::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Interrup::OnTermination()
{
	Super::OnTermination();
}
