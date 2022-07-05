// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Attack::UDWCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWCharacterState_Attack::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Attack::OnValidate()
{
	return Super::OnValidate();
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim(true, true);
	Character->SetMotionRate(0, 0);
}

void UDWCharacterState_Attack::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Attack::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->UnAttack();
}

void UDWCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}
