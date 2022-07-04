// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Crouch.h"

UDWCharacterState_Crouch::UDWCharacterState_Crouch()
{
	StateName = FName("Crouch");
}

void UDWCharacterState_Crouch::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Crouch::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Crouch::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Crouch::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Crouch::OnTermination()
{
	Super::OnTermination();
}
