// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Jump.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Jump::UDWCharacterState_Jump()
{
	StateName = FName("Jump");
}

void UDWCharacterState_Jump::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Jump::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return !Character->IsSwimming() && Character->DoAction(GameplayTags::Ability_Character_Action_Jump);
}

void UDWCharacterState_Jump::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim();
}

void UDWCharacterState_Jump::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Jump::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Jump);

	Character->FreeToAnim();
}

void UDWCharacterState_Jump::OnTermination()
{
	Super::OnTermination();
}
