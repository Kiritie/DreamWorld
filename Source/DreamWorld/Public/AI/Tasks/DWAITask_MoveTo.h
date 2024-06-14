// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/Tasks/AITask_Duration.h"
#include "DWAITask_MoveTo.generated.h"

class ADWCharacter;
/**
 * AI任务_自动寻路
 */
UCLASS()
class DREAMWORLD_API UDWAITask_MoveTo : public UAITask_Duration
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MoveTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MoveDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MovementRate;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RotationRate;

private:
	UPROPERTY()
	ADWCharacter* MoveTarget;

	FVector MoveLocation;
	
	float MoveDistance;
};
