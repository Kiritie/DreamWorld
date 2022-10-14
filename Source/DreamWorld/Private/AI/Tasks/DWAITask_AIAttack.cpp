// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_AIAttack.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "AI/DWAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDWAITask_AIAttack::UDWAITask_AIAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttackTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIAttack, AttackTargetKey), ADWCharacter::StaticClass());
	AttackDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIAttack, AttackDistanceKey));

	AttackTarget = nullptr;
	AttackDistance = 0.f;
}

bool UDWAITask_AIAttack::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	AttackTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	AttackDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackDistanceKey.SelectedKeyName);

	return AttackTarget && AttackTarget->IsValidLowLevel();
}

void UDWAITask_AIAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	if(GetOwnerCharacter<ADWCharacter>()->GetDistance(AttackTarget, false, false) <= AttackDistance)
	{
		if(!AttackTarget->IsDead())
		{
			GetOwnerCharacter<ADWCharacter>()->Attack();
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UDWAITask_AIAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetOwnerCharacter<ADWCharacter>()->UnAttack();
	//GetOwnerCharacter<ADWCharacter>()->SetLockedTarget(nullptr);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_AIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetOwnerCharacter<ADWCharacter>()->SetLockedTarget(AttackTarget);

	return EBTNodeResult::InProgress;
}

void UDWAITask_AIAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetOwnerCharacter<ADWCharacter>()->UnAttack();
	GetOwnerCharacter<ADWCharacter>()->SetLockedTarget(nullptr);
}
