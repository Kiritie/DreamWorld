// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/DWAITask.h"
#include "DWAITask_AIFollow.generated.h"

class ADWCharacter;

/**
 * AI任务_跟随
 */
UCLASS()
class DREAMWORLD_API UDWAITask_AIFollow : public UDWAITask
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetCharacterKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetDistanceKey;

private:
	UPROPERTY()
	ADWCharacter* TargetCharacter;

	float TargetDistance;
	
protected:
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
