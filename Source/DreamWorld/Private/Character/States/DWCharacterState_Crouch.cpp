// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Crouch.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Crouch::UDWCharacterState_Crouch()
{
	StateName = FName("Crouch");
}

void UDWCharacterState_Crouch::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Crouch::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::AbilityTag_Character_Action_Crouch);
}

void UDWCharacterState_Crouch::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Crouching);

	Character->Crouch(InParams[0]);
}

void UDWCharacterState_Crouch::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Crouch::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::AbilityTag_Character_Action_Crouch);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Crouching);

	Character->FreeToAnim();
}

void UDWCharacterState_Crouch::OnTermination()
{
	Super::OnTermination();
}
