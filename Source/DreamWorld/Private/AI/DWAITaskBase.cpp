// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAITaskBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"

UDWAITaskBase::UDWAITaskBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDWAITaskBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!InitTask(OwnerComp)) return;

}

EBTNodeResult::Type UDWAITaskBase::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITaskBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}

void UDWAITaskBase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if(!InitTask(OwnerComp)) return;

}
