// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Swim.h"

#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "Scene/SceneModuleStatics.h"

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

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->ModifyOxygen(-Character->GetOxygenExpendSpeed() * DeltaSeconds);
}

void UDWCharacterState_Swim::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Swim::OnTermination()
{
	Super::OnTermination();
}
