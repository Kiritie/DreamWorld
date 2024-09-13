// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Death.h"

#include "Character/DWCharacter.h"
#include "Ability/Character/AbilityCharacterBase.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Common/Looking/LookingComponent.h"

UDWCharacterState_Death::UDWCharacterState_Death()
{
	
}

void UDWCharacterState_Death::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Death::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Death::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->SetMana(0.f);
	Character->SetStamina(0.f);
	Character->GetLooking()->TargetLookingOff();
	Character->LimitToAnim();
}

void UDWCharacterState_Death::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Death::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Death);
}

void UDWCharacterState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Death::DeathStart()
{
	Super::DeathStart();

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(EDWCharacterActionType::Death);
}

void UDWCharacterState_Death::DeathEnd()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Vitality_Dying);
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Vitality_Dead);

	Character->StopAction(EDWCharacterActionType::Death);

	Character->Inventory->DiscardAllItem();

	if(!Character->IsPlayer())
	{
		UObjectPoolModuleStatics::DespawnObject(Character);
	}
	else
	{
		Character->Execute_SetActorVisible(Character, false);
	}
}
