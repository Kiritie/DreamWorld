// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fly.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "FSM/Components/FSMComponent.h"
#include "Scene/SceneModuleStatics.h"

UDWCharacterState_Fly::UDWCharacterState_Fly()
{
	
}

void UDWCharacterState_Fly::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Fly::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Fly::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWCharacterState_Fly::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Character, Stamina, -Character->GetStaminaExpendSpeed() * 2.5f * DeltaSeconds));
}

void UDWCharacterState_Fly::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Fly::OnTermination()
{
	Super::OnTermination();
}
