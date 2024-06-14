// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Dodge.h"

#include "Character/Player/DWPlayerCharacter.h"

UDWPlayerCharacterState_Dodge::UDWPlayerCharacterState_Dodge()
{
	StateName = FName("Dodge");
}

void UDWPlayerCharacterState_Dodge::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Dodge::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnEnterValidate(InLastState, InParams);
}

void UDWPlayerCharacterState_Dodge::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	// PlayerCharacter->GetTargeting()->SetShouldControlRotation(false);
}

void UDWPlayerCharacterState_Dodge::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Dodge::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	// PlayerCharacter->GetTargeting()->SetShouldControlRotation(true);
}

void UDWPlayerCharacterState_Dodge::OnTermination()
{
	Super::OnTermination();
}
