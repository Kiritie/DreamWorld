// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Attack.h"

#include "TimerManager.h"

UDWPlayerCharacterState_Attack::UDWPlayerCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWPlayerCharacterState_Attack::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Attack::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Attack::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWPlayerCharacterState_Attack::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Attack::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
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
