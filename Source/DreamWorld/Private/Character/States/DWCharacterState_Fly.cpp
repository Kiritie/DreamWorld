// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fly.h"

UDWCharacterState_Fly::UDWCharacterState_Fly()
{
	StateName = FName("Fly");
}

void UDWCharacterState_Fly::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Fly::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Fly::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Fly::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Fly::OnTermination()
{
	Super::OnTermination();
}
