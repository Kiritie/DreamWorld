// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Death.h"

#include "Character/DWCharacter.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Ability/Character/AbilityCharacterBase.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/Character/AbilityCharacterInventoryBase.h"

UDWCharacterState_Death::UDWCharacterState_Death()
{
	
}

void UDWCharacterState_Death::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Death::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWCharacterState_Death::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->SetMana(0.f);
	Character->SetStamina(0.f);
	Character->SetLockedTarget(nullptr);
	Character->LimitToAnim();
}

void UDWCharacterState_Death::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Death::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

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
