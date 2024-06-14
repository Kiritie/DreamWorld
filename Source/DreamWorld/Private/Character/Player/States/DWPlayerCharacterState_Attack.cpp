// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Attack.h"

#include "TimerManager.h"

UDWPlayerCharacterState_Attack::UDWPlayerCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWPlayerCharacterState_Attack::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Attack::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnEnterValidate(InLastState, InParams);
}

void UDWPlayerCharacterState_Attack::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Attack::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Attack::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWPlayerCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Attack::AttackStart()
{
	Super::AttackStart();
}

void UDWPlayerCharacterState_Attack::AttackStep()
{
	Super::AttackStep();
}

void UDWPlayerCharacterState_Attack::AttackEnd()
{
	Super::AttackEnd();
}
