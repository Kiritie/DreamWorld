// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Interrupt.h"

#include "Character/DWCharacterData.h"
#include "FSM/Components/FSMComponent.h"

UDWCharacterState_Interrupt::UDWCharacterState_Interrupt()
{
	
}

void UDWCharacterState_Interrupt::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Interrupt::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Interrupt::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWCharacterState_Interrupt::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Interrupt::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Interrupt::OnTermination()
{
	Super::OnTermination();
}
