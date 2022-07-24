// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Jump.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Jump::UDWCharacterState_Jump()
{
	StateName = FName("Jump");
}

void UDWCharacterState_Jump::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Jump::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Jump);
}

void UDWCharacterState_Jump::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Jump::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Jump::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Jump);
}

void UDWCharacterState_Jump::OnTermination()
{
	Super::OnTermination();
}
