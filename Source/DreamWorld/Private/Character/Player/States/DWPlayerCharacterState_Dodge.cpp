// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Dodge.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Components/CapsuleComponent.h"

UDWPlayerCharacterState_Dodge::UDWPlayerCharacterState_Dodge()
{
	StateName = FName("Dodge");
}

void UDWPlayerCharacterState_Dodge::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Dodge::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Dodge::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	// PlayerCharacter->GetTargetSystem()->SetShouldControlRotation(false);
}

void UDWPlayerCharacterState_Dodge::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Dodge::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	// PlayerCharacter->GetTargetSystem()->SetShouldControlRotation(true);
}

void UDWPlayerCharacterState_Dodge::OnTermination()
{
	Super::OnTermination();
}
