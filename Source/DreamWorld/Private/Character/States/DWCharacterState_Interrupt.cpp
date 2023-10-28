// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Interrupt.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"

UDWCharacterState_Interrupt::UDWCharacterState_Interrupt()
{
	StateName = FName("Interrupt");

	Duration = 0.f;
	RemainTime = 0.f;
}

void UDWCharacterState_Interrupt::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Interrupt::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Interrupt);
}

void UDWCharacterState_Interrupt::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	RemainTime = Duration;

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Interrupting);

	Character->LimitToAnim();
}

void UDWCharacterState_Interrupt::OnRefresh()
{
	Super::OnRefresh();

	if (RemainTime != -1)
	{
		RemainTime -= GetWorld()->GetDeltaSeconds();
		if (RemainTime <= 0)
		{
			FSM->SwitchState(nullptr);
		}
	}
}

void UDWCharacterState_Interrupt::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Interrupt);
		
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Interrupting);

	Character->FreeToAnim();
}

void UDWCharacterState_Interrupt::OnTermination()
{
	Super::OnTermination();
}
