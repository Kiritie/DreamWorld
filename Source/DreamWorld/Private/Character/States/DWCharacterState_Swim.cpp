// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Swim.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Scene/SceneModuleStatics.h"
#include "Character/States/DWCharacterState_Float.h"

UDWCharacterState_Swim::UDWCharacterState_Swim()
{
	
}

void UDWCharacterState_Swim::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Swim::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Swim::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWCharacterState_Swim::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Swim::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Swim::OnTermination()
{
	Super::OnTermination();
}
