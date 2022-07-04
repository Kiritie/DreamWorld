// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Float.h"

UDWCharacterState_Float::UDWCharacterState_Float()
{
	StateName = FName("Float");
}

void UDWCharacterState_Float::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Float::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Float::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Float::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Float::OnTermination()
{
	Super::OnTermination();
}
