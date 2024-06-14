// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fall.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Fall::UDWCharacterState_Fall()
{
	StateName = FName("Fall");
}

void UDWCharacterState_Fall::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Fall::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Fall);
}

void UDWCharacterState_Fall::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim();
}

void UDWCharacterState_Fall::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Fall::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();

	Character->StopAction(EDWCharacterActionType::Fall);
}

void UDWCharacterState_Fall::OnTermination()
{
	Super::OnTermination();
}
