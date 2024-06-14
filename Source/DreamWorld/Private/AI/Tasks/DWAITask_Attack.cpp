// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_Attack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/Looking/LookingComponent.h"

UDWAITask_Attack::UDWAITask_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttackTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_Attack, AttackTargetKey), ADWCharacter::StaticClass());
	AttackDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_Attack, AttackDistanceKey));

	AttackTarget = nullptr;
	AttackDistance = 0.f;
}

void UDWAITask_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	AttackTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	AttackTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_Attack::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	AttackTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	AttackDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackDistanceKey.SelectedKeyName);

	return AttackTarget && AttackTarget->IsValidLowLevel();
}

void UDWAITask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	if(GetAgent<ADWCharacter>()->GetDistance(AttackTarget, false, false) <= AttackDistance)
	{
		if(!AttackTarget->IsDead())
		{
			GetAgent<ADWCharacter>()->Attack();
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else if(GetAgent<ADWCharacter>()->IsFreeToAnim())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UDWAITask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetAgent<ADWCharacter>()->UnAttack();
	//GetAgent<ADWCharacter>()->SetLockedTarget(nullptr);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOn(AttackTarget);

	return EBTNodeResult::InProgress;
}

void UDWAITask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnAttack();
	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOff();
}
