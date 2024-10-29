// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Base/AIServiceBase.h"
#include "DWAIService_QueryPatrolPath.generated.h"

/**
 * AI任务_巡逻
 */
UCLASS()
class DREAMWORLD_API UDWAIService_QueryPatrolPath : public UAIServiceBase
{
	GENERATED_BODY()
	
public:
	UDWAIService_QueryPatrolPath(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitService(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolDurationKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolLocationKey;
};
