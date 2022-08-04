// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIBlackboard.h"

#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"

void UDWAIBlackboard::PostLoad()
{
	Super::PostLoad();

	// BLACKBOARD_VALUE_GENERATE_ENUM(CharacterNature, EDWCharacterNature);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(AttackDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(FollowDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDuration);
}

void UDWAIBlackboard::OnInitialize(UBlackboardComponent* InComponent, ACharacterBase* InCharacter)
{
	Super::OnInitialize(InComponent, InCharacter);
}

void UDWAIBlackboard::OnRefresh()
{
	Super::OnRefresh();

	if(!GetCharacter<ADWCharacter>()) return;

	SetCharacterNature((uint8)GetCharacter<ADWCharacter>()->GetNature());
	SetAttackDistance(GetCharacter<ADWCharacter>()->GetAttackDistance());
	SetFollowDistance(GetCharacter<ADWCharacter>()->GetFollowDistance());
	SetPatrolDistance(GetCharacter<ADWCharacter>()->GetPatrolDistance());
	SetPatrolDuration(GetCharacter<ADWCharacter>()->GetPatrolDuration());

	if(GetTargetCharacter<ADWCharacter>())
	{
		if(GetCharacter<ADWCharacter>()->IsDead() || GetTargetCharacter<ADWCharacter>()->IsDead())
		{
			SetTargetCharacter(nullptr);
		}
	}
}

void UDWAIBlackboard::OnValuePreChange(FName InValueName)
{
	Super::OnValuePreChange(InValueName);
}

void UDWAIBlackboard::OnValueChanged(FName InValueName)
{
	Super::OnValueChanged(InValueName);
}
