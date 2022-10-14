// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Defend.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Defend::UDWCharacterState_Defend()
{
	StateName = FName("Defend");
}

void UDWCharacterState_Defend::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Defend::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->ControlMode == EDWCharacterControlMode::Fighting && Character->DoAction(EDWCharacterActionType::Defend);
}

void UDWCharacterState_Defend::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().DefendingTag);

	Character->SetMotionRate(0.5f, 0.1f);
	Character->LimitToAnim();
}

void UDWCharacterState_Defend::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Defend::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Defend);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().DefendingTag);

	Character->FreeToAnim();
	Character->SetMotionRate(1, 1);
}

void UDWCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
