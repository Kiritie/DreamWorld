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
	// BLACKBOARD_VALUE_GENERATE_OBJECT(TargetCharacter, ADWCharacter);
	// BLACKBOARD_VALUE_GENERATE_BOOL(IsLostTarget);
	// BLACKBOARD_VALUE_GENERATE_VECTOR(LostTargetLocation);
}

void UDWAIBlackboard::Initialize(UBlackboardComponent* InComponent, ACharacterBase* InCharacter)
{
	Super::Initialize(InComponent, InCharacter);

	SetTargetCharacter(nullptr);
	SetIsLostTarget(false);
	SetLostTargetLocation(FVector::ZeroVector);
}

void UDWAIBlackboard::Refresh()
{
	Super::Refresh();

	SetCharacterNature((uint8)GetCharacter<ADWCharacter>()->GetNature());
	SetAttackDistance(GetCharacter<ADWCharacter>()->GetAttackDistance());
	SetFollowDistance(GetCharacter<ADWCharacter>()->GetFollowDistance());
	SetPatrolDistance(GetCharacter<ADWCharacter>()->GetPatrolDistance());
	SetPatrolDuration(GetCharacter<ADWCharacter>()->GetPatrolDuration());
}
