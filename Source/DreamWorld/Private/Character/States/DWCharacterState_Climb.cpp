// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Climb.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Climb::UDWCharacterState_Climb()
{
	StateName = FName("Climb");
}

void UDWCharacterState_Climb::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Climb::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Climb);
}

void UDWCharacterState_Climb::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().ClimbingTag);

	Character->LimitToAnim();
}

void UDWCharacterState_Climb::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Climb::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Climb);
		
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().ClimbingTag);

	Character->FreeToAnim();
}

void UDWCharacterState_Climb::OnTermination()
{
	Super::OnTermination();
}
