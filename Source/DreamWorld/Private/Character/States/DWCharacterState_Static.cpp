// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Static.h"

#include "AbilitySystemComponent.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UDWCharacterState_Static::UDWCharacterState_Static()
{
	StateName = FName("Static");
}

void UDWCharacterState_Static::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Static::OnValidate()
{
	return Super::OnValidate();
}

void UDWCharacterState_Static::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Static::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Static::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Static::OnTermination()
{
	Super::OnTermination();
}
