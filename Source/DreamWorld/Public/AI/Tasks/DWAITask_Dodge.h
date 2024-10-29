// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWAITask_Fight.h"
#include "DWAITask_Dodge.generated.h"

class ADWCharacter;
/**
 * AI任务_防御
 */
UCLASS()
class DREAMWORLD_API UDWAITask_Dodge : public UDWAITask_Fight
{
	GENERATED_BODY()
	
public:
	UDWAITask_Dodge(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
