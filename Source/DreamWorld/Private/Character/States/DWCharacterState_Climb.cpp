// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Climb.h"

UDWCharacterState_Climb::UDWCharacterState_Climb()
{
	StateName = FName("Climb");
}

void UDWCharacterState_Climb::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Climb::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Climb::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Climb::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Climb::OnTermination()
{
	Super::OnTermination();
}
