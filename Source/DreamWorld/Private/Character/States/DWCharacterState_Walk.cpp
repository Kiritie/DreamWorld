// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Walk.h"

#include "Ability/Effects/EffectBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Fall.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"

UDWCharacterState_Walk::UDWCharacterState_Walk()
{
	StateName = FName("Walk");
}

void UDWCharacterState_Walk::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Walk::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::AbilityTag_Character_Action_Walk);
}

void UDWCharacterState_Walk::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(InLastState && InLastState->IsA<UDWCharacterState_Fall>())
	{
		UEffectBase* Effect = UObjectPoolModuleStatics::SpawnObject<UEffectBase>();

		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.Attribute = GET_GAMEPLAYATTRIBUTE_PROPERTY(UVitalityAttributeSetBase, FallDamage);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(1.f);

		Effect->Modifiers.Add(ModifierInfo);
		
		FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(Character);
		Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Effect, 0, EffectContext);

		UObjectPoolModuleStatics::DespawnObject(Effect);
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
	
	Character->StopAction(GameplayTags::AbilityTag_Character_Action_Walk);
}

void UDWCharacterState_Walk::OnTermination()
{
	Super::OnTermination();
}
