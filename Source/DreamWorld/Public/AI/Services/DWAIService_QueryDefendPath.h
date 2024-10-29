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
	GENERATED_BODY()

public:
	UDWAIService_QueryDefendPath(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitService(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void RefreshDefendLocation(UBehaviorTreeComponent& OwnerComp);

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DefendTargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DefendLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float DefendDistanceOffset;

private:
	UPROPERTY()
	ADWCharacter* DefendTarget;
};
