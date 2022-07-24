// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Death.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/Inventory.h"
#include "Voxel/Chunks/VoxelChunk.h"

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
}

void UDWCharacterState_Death::OnRefresh()
{
	Super::OnRefresh();

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->IsDying() && !Character->IsFalling())
	{
		DeathStart();
	}
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
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(EDWCharacterActionType::Death);
}

void UDWCharacterState_Death::DeathEnd()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Death);

	Character->Inventory->DiscardAllItem();

	if(!Character->IsPlayer())
	{
		if(Character->GetController())
		{
			Character->GetController()->UnPossess();
		}
		if (Character->HasTeam())
		{
			Character->GetTeamData()->RemoveMember(Character);
		}
		for(auto Iter : Character->Equips)
		{
			if(Iter.Value)
			{
				Iter.Value->Destroy();
			}
		}
	}

	Super::DeathEnd();
}
