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

bool UDWCharacterState_Defend::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->ControlMode == EDWCharacterControlMode::Fighting && Character->DoAction(EDWCharacterActionType::Defend);
}

void UDWCharacterState_Defend::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Defending);

	Character->SetMotionRate(0.5f, 0.1f);
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

	Character->StopAction(EDWCharacterActionType::Defend);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Defending);

	Character->FreeToAnim();
	Character->SetMotionRate(1, 1);
}

void UDWCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
