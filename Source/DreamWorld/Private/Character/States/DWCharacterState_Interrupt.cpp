// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Interrupt.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UDWCharacterState_Interrupt::UDWCharacterState_Interrupt()
{
	StateName = FName("Interrupt");

	Duration = 0.f;
	RemainTime = 0.f;
}

void UDWCharacterState_Interrupt::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Interrupt::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Interrupt);
}

void UDWCharacterState_Interrupt::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	if(InParams.IsValidIndex(0))
	{
		Duration = InParams[0];
		RemainTime = Duration;
	}

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Interrupting);

	Character->GetCharacterMovement()->SetActive(false);

	Character->LimitToAnim();
}

void UDWCharacterState_Interrupt::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	RemainTime -= GetWorld()->GetDeltaSeconds();
	if (RemainTime <= 0.f)
	{
		FSM->RefreshState();
	}
}

void UDWCharacterState_Interrupt::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Interrupt);
		
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Interrupting);

	Character->GetCharacterMovement()->SetActive(true);

	Character->FreeToAnim();
}

void UDWCharacterState_Interrupt::OnTermination()
{
	Super::OnTermination();
}
