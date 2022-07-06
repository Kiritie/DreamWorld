// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Character/Player/DWPlayerCharacter.h"

UDWPlayerCharacterState_Attack::UDWPlayerCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWPlayerCharacterState_Attack::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Attack::OnValidate()
{
	return Super::OnValidate();
}

void UDWPlayerCharacterState_Attack::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWPlayerCharacterState_Attack::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Attack::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
	
	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->AttackAbilityQueue = 0;
}

void UDWPlayerCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Attack::AttackStart()
{
	Super::AttackStart();
	
	if (!IsCurrentState()) return;
	
	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	switch (PlayerCharacter->GetAttackType())
	{
		case EDWCharacterAttackType::NormalAttack:
		case EDWCharacterAttackType::FallingAttack:
		{
			if(PlayerCharacter->AttackAbilityQueue > 0)
			{
				PlayerCharacter->AttackAbilityQueue--;
			}
			break;
		}
		default: break;
	}

}

void UDWPlayerCharacterState_Attack::AttackHurt()
{
	Super::AttackHurt();
}

void UDWPlayerCharacterState_Attack::AttackEnd()
{
	Super::AttackEnd();
}
