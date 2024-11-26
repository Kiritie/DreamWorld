// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIBlackboard.h"

#include "AI/Base/AIControllerBase.h"
#include "Character/DWCharacter.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/World/WidgetCharacterHP.h"

void UDWAIBlackboard::PostLoad()
{
	Super::PostLoad();
	
	// BLACKBOARD_VALUE_GENERATE_BOOL(IsExcessived);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(AttackDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(FollowDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDistance);
	// BLACKBOARD_VALUE_GENERATE_FLOAT(PatrolDuration);
}

void UDWAIBlackboard::Initialize(UBlackboardComponent* InComponent)
{
	Super::Initialize(InComponent);
}

void UDWAIBlackboard::OnReset()
{
	Super::OnReset();

	ResetIsExcessived();
	ResetAttackDistance();
	ResetFollowDistance();
	ResetPatrolDistance();
	ResetPatrolDuration();
}

void UDWAIBlackboard::OnRefresh()
{
	Super::OnRefresh();

	if(!GetAgent<ADWCharacter>()) return;

	if(GetTargetAgent<ADWCharacter>() && GetTargetAgent<ADWCharacter>()->IsDead())
	{
		ResetIsLostTarget();
		ResetTargetLocation();
		ResetTargetAgent();
		ResetIsExcessived();
	}
}

void UDWAIBlackboard::OnValueReset(FName InValueName)
{
	Super::OnValueReset(InValueName);

	if(InValueName.IsEqual(NAME_IsExcessived))
	{
		SetIsExcessived(false);
	}
	else if(InValueName.IsEqual(NAME_AttackDistance))
	{
		SetAttackDistance(GetAgent<ADWCharacter>()->GetAttackDistance());
	}
	else if(InValueName.IsEqual(NAME_FollowDistance))
	{
		SetFollowDistance(GetAgent<ADWCharacter>()->GetFollowDistance());
	}
	else if(InValueName.IsEqual(NAME_PatrolDistance))
	{
		SetPatrolDistance(GetAgent<ADWCharacter>()->GetPatrolDistance());
	}
	else if(InValueName.IsEqual(NAME_PatrolDuration))
	{
		SetPatrolDuration(GetAgent<ADWCharacter>()->GetPatrolDuration());
	}
}

void UDWAIBlackboard::OnValuePreChange(FName InValueName)
{
	Super::OnValuePreChange(InValueName);
}

void UDWAIBlackboard::OnValueChanged(FName InValueName)
{
	Super::OnValueChanged(InValueName);

	if(InValueName.IsEqual(NAME_IsExcessived))
	{
		GetController()->InitBehaviorTree(true);
	}
}
