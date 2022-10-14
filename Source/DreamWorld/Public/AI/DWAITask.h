// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "AI/Base/AITaskBase.h"
#include "DWAITask.generated.h"

/**
 * AI任务基类
 */
UCLASS()
class DREAMWORLD_API UDWAITask : public UAITaskBase
{
	GENERATED_UCLASS_BODY()
		
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float DurationTime;

private:
	float LocalRemainTime;

protected:
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
