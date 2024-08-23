// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Walk.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Fall.h"

UDWCharacterState_Walk::UDWCharacterState_Walk()
{
	StateName = FName("Walk");
}

void UDWCharacterState_Walk::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Walk::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Walk);
}

void UDWCharacterState_Walk::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(InLastState && InLastState->IsA<UDWCharacterState_Fall>())
	{
		const auto& CharacterData = Character->GetCharacterData<UDWCharacterData>();
		if(CharacterData.FallDamageClass)
		{
			auto EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			auto SpecHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(CharacterData.FallDamageClass, 0, EffectContext);
			if (SpecHandle.IsValid())
			{
				Character->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(SpecHandle);
			}
		}
	}
}

void UDWCharacterState_Walk::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Walk::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->StopAction(EDWCharacterActionType::Walk);
}

void UDWCharacterState_Walk::OnTermination()
{
	Super::OnTermination();
}
