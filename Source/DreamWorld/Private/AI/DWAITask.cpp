// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAITask.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"

UDWAITask::UDWAITask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDWAITask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!InitTask(OwnerComp)) return;

}

EBTNodeResult::Type UDWAITask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}

void UDWAITask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if(!InitTask(OwnerComp)) return;

}
