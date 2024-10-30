// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Static.h"

#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"

UDWCharacterState_Static::UDWCharacterState_Static()
{
	
}

void UDWCharacterState_Static::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Static::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Static::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWCharacterState_Static::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Static::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Static::OnTermination()
{
	Super::OnTermination();
}
