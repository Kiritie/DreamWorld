// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Interrupt.h"

UDWPlayerCharacterState_Interrupt::UDWPlayerCharacterState_Interrupt()
{
	StateName = FName("Interrupt");
}

void UDWPlayerCharacterState_Interrupt::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Interrupt::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Interrupt::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWPlayerCharacterState_Interrupt::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Interrupt::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWPlayerCharacterState_Interrupt::OnTermination()
{
	Super::OnTermination();
}
