// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Defend.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Defend::UDWCharacterState_Defend()
{
	StateName = FName("Defend");
}

void UDWCharacterState_Defend::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Defend::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Defend);
}

void UDWCharacterState_Defend::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Defending);

	Character->SetMotionRate(0.5f, 0.5f);
	Character->LimitToAnim();
}

void UDWCharacterState_Defend::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Defend::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Defend);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Defending);

	Character->FreeToAnim();
	Character->SetMotionRate(1.f, 1.f);
}

void UDWCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
