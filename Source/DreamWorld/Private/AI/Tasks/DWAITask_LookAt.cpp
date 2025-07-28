// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_LookAt.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Common/Looking/LookingComponent.h"

UDWAITask_LookAt::UDWAITask_LookAt(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LookAtTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_LookAt, LookAtTargetKey), ADWCharacter::StaticClass());

	LookAtTargetKey.AllowNoneAsValue(true);
	
	LookAtTarget = nullptr;
}

void UDWAITask_LookAt::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	LookAtTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_LookAt::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if (!Super::InitTask(OwnerComp)) return false;
	
	LookAtTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(LookAtTargetKey.SelectedKeyName));
	return true;
}

void UDWAITask_LookAt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(!InitTask(OwnerComp)) return;

	if(LookAtTarget)
	{
		GetAgent<ADWCharacter>()->GetLooking()->DoLookAtTarget(LookAtTarget);
	}
}

EBTNodeResult::Type UDWAITask_LookAt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UDWAITask_LookAt::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;
}
