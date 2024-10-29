// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/Tasks/AITask_Duration.h"
#include "DWAITask_Fight.generated.h"

class ADWCharacter;
/**
 * AI任务_看向目标
 */
UCLASS(Abstract)
class DREAMWORLD_API UDWAITask_Fight : public UAITask_Duration
{
	GENERATED_BODY()
	
public:
	UDWAITask_Fight(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FightTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bLookingTarget;

	UPROPERTY()
	ADWCharacter* FightTarget;
};
