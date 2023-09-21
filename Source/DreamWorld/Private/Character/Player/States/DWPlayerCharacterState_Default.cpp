// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Default.h"

#include "Character/Player/DWPlayerCharacter.h"


UDWPlayerCharacterState_Default::UDWPlayerCharacterState_Default()
{
	
}

void UDWPlayerCharacterState_Default::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Default::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Default::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();
}

void UDWPlayerCharacterState_Default::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Default::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWPlayerCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}
