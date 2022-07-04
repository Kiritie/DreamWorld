// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Death.h"

UDWCharacterState_Death::UDWCharacterState_Death()
{
	
}

void UDWCharacterState_Death::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Death::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Death::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Death::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Death::DeathStart()
{
	Super::DeathStart();
}

void UDWCharacterState_Death::DeathEnd()
{
	Super::DeathEnd();
}
