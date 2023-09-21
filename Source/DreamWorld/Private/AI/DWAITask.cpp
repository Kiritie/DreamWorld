// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAITask.h"

UDWAITask::UDWAITask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DurationTime = -1.f;
	LocalRemainTime = 0.f;
}

bool UDWAITask::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	return Super::InitTask(OwnerComp);
}

void UDWAITask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!InitTask(OwnerComp)) return;

	if(LocalRemainTime != -1.f)
	{
		LocalRemainTime -= DeltaSeconds;
		if(LocalRemainTime <= 0.f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UDWAITask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	LocalRemainTime = DurationTime;

	return EBTNodeResult::Succeeded;
}

void UDWAITask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if(!InitTask(OwnerComp)) return;

}
