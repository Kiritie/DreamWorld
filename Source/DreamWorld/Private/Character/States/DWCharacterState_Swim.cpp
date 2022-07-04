// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Swim.h"

UDWCharacterState_Swim::UDWCharacterState_Swim()
{
	StateName = FName("Swim");
}

void UDWCharacterState_Swim::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Swim::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Swim::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Swim::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Swim::OnTermination()
{
	Super::OnTermination();
}
