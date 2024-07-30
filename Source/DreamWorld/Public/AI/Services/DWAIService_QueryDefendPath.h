// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Base/AIServiceBase.h"
#include "DWAIService_QueryDefendPath.generated.h"

class ADWCharacter;
/**
 * AI任务_巡逻
 */
UCLASS()
class DREAMWORLD_API UDWAIService_QueryDefendPath : public UAIServiceBase
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitService(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DefendTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DefendDistanceKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DefendLocationKey;

private:
	UPROPERTY()
	ADWCharacter* DefendTarget;

	float DefendDistance;
};
