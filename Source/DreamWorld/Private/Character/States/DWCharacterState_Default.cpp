// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

UDWCharacterState_Attack::UDWCharacterState_Attack()
{
	
}

void UDWCharacterState_Attack::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Attack::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Attack::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}
