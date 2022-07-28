// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Crouch.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Crouch::UDWCharacterState_Crouch()
{
	StateName = FName("Crouch");
}

void UDWCharacterState_Crouch::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Crouch::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Crouch);
}

void UDWCharacterState_Crouch::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().CrouchingTag);

	Character->LimitToAnim();
}

void UDWCharacterState_Crouch::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Crouch::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Crouch);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().CrouchingTag);

	Character->FreeToAnim();
}

void UDWCharacterState_Crouch::OnTermination()
{
	Super::OnTermination();
}
