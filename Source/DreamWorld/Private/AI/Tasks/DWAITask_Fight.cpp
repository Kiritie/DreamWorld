// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_Fight.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Common/Looking/LookingComponent.h"

UDWAITask_Fight::UDWAITask_Fight(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FightTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_Fight, FightTargetKey), ADWCharacter::StaticClass());

	bLookingTarget = false;
	FightTarget = nullptr;
}

void UDWAITask_Fight::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	FightTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_Fight::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if (!Super::InitTask(OwnerComp)) return false;
	
	FightTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FightTargetKey.SelectedKeyName));
	return FightTarget && FightTarget->IsValidLowLevel();
}

void UDWAITask_Fight::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(!InitTask(OwnerComp)) return;
}

EBTNodeResult::Type UDWAITask_Fight::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_Fight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	if(bLookingTarget)
	{
		GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOn(FightTarget);
	}

	return EBTNodeResult::InProgress;
}

void UDWAITask_Fight::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	if(bLookingTarget)
	{
		GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOff();
	}
}
