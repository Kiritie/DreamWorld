// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Human/States/DWHumanCharacterState_Defend.h"

#include "Character/DWCharacter.h"
#include "Item/Equip/Shield/DWEquipShield.h"

UDWHumanCharacterState_Defend::UDWHumanCharacterState_Defend()
{
}

void UDWHumanCharacterState_Defend::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWHumanCharacterState_Defend::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->GetShield() && Super::OnPreEnter(InLastState, InParams);
}

void UDWHumanCharacterState_Defend::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWHumanCharacterState_Defend::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWHumanCharacterState_Defend::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWHumanCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
