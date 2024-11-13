// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Float.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/DWCharacterPart.h"
#include "FSM/Components/FSMComponent.h"

UDWCharacterState_Float::UDWCharacterState_Float()
{
	
}

void UDWCharacterState_Float::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Float::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Float::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWCharacterState_Float::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Float::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Float::OnTermination()
{
	Super::OnTermination();
}

float UDWCharacterState_Float::GetFloatOffsetPosZ() const
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	const float NeckPosZ = Character->GetCharacterPart(EDWCharacterPart::Neck)->GetComponentLocation().Z;
	const float ChestPosZ = Character->GetCharacterPart(EDWCharacterPart::Chest)->GetComponentLocation().Z;
	const float OffsetZ = ChestPosZ + (NeckPosZ - ChestPosZ) * 0.5f - Character->GetActorLocation().Z;
	return OffsetZ;
}
