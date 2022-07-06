// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Walk.h"

#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Attack.h"

UDWCharacterState_Walk::UDWCharacterState_Walk()
{
	StateName = FName("Walk");
}

void UDWCharacterState_Walk::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Walk::OnValidate()
{
	return Super::OnValidate();
}

void UDWCharacterState_Walk::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
}

void UDWCharacterState_Walk::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Walk::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Walk::OnTermination()
{
	Super::OnTermination();
}
