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

bool UDWCharacterState_Walk::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Walk);
}

void UDWCharacterState_Walk::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Walk::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Walk::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->StopAction(EDWCharacterActionType::Walk);
}

void UDWCharacterState_Walk::OnTermination()
{
	Super::OnTermination();
}
