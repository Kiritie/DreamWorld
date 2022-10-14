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
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Fall);
}

void UDWCharacterState_Fall::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim();
}

void UDWCharacterState_Fall::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Fall::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();

	Character->StopAction(EDWCharacterActionType::Fall);
}

void UDWCharacterState_Fall::OnTermination()
{
	Super::OnTermination();
}
