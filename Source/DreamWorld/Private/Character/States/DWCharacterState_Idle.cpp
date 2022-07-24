// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Idle.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Idle::UDWCharacterState_Idle()
{
	StateName = FName("Idle");
}

void UDWCharacterState_Idle::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Idle::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWCharacterState_Idle::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Idle::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Idle::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Idle::OnTermination()
{
	Super::OnTermination();
}
