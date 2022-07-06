// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Default.h"

#include "Character/DWCharacter.h"

UDWCharacterState_Default::UDWCharacterState_Default()
{
	
}

void UDWCharacterState_Default::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Default::OnValidate()
{
	return Super::OnValidate();
}

void UDWCharacterState_Default::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(EDWCharacterActionType::Revive);

	Character->SetMana(-1.f);
	Character->SetStamina(-1.f);
	
	// stats
	Character->SetLockedTarget(nullptr);
	Character->RidingTarget = nullptr;
	Character->OwnerRider = nullptr;
	
	// local
	Character->AttackAbilityIndex = 0;
	Character->AIMoveLocation = Vector_Empty;
	Character->AIMoveStopDistance = 0;
	Character->NormalAttackRemainTime = 0;
	Character->ActionType = EDWCharacterActionType::None;
}

void UDWCharacterState_Default::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Default::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}
