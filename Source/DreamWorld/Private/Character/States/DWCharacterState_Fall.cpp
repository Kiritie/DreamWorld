// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fall.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Fall::UDWCharacterState_Fall()
{
	StateName = FName("Fall");

	MaxPosZ = 0.f;
}

void UDWCharacterState_Fall::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Fall::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Fall);
}

void UDWCharacterState_Fall::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim();

	MaxPosZ = 0.f;
}

void UDWCharacterState_Fall::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	MaxPosZ = FMath::Max(Character->GetActorLocation().Z, MaxPosZ);
}

void UDWCharacterState_Fall::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();

	Character->StopAction(GameplayTags::Ability_Character_Action_Fall);
}

void UDWCharacterState_Fall::OnTermination()
{
	Super::OnTermination();
}

float UDWCharacterState_Fall::GetFallHeight() const
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return MaxPosZ - Character->GetActorLocation().Z;
}
