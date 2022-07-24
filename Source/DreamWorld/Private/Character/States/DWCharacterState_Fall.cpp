// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fall.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Fall::UDWCharacterState_Fall()
{
	StateName = FName("Fall");
}

void UDWCharacterState_Fall::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Fall::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWCharacterState_Fall::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Fall::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Fall::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Fall::OnTermination()
{
	Super::OnTermination();
}
