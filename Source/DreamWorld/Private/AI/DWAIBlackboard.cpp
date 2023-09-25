// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIBlackboard.h"

#include "Character/DWCharacter.h"

void UDWAIBlackboard::PostLoad()
{
	Super::PostLoad();

	// BLACKBOARD_VALUE_GENERATE_ENUM(CharacterNature, EDWCharacterNature);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(AttackDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(FollowDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDuration);
}

void UDWAIBlackboard::Initialize(UBlackboardComponent* InComponent, IAIAgentInterface* InAgent)
{
	Super::Initialize(InComponent, InAgent);

	if(!GetAgent<ADWCharacter>()) return;

	SetCharacterNature((uint8)GetAgent<ADWCharacter>()->GetNature());
	SetAttackDistance(GetAgent<ADWCharacter>()->GetAttackDistance());
	SetFollowDistance(GetAgent<ADWCharacter>()->GetFollowDistance());
	SetPatrolDistance(GetAgent<ADWCharacter>()->GetPatrolDistance());
	SetPatrolDuration(GetAgent<ADWCharacter>()->GetPatrolDuration());
}

void UDWAIBlackboard::OnRefresh()
{
	Super::OnRefresh();

	if(!GetAgent<ADWCharacter>()) return;

	if(GetTargetAgent<ADWCharacter>() && GetTargetAgent<ADWCharacter>()->IsDead())
	{
		SetTargetAgent(nullptr);
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
