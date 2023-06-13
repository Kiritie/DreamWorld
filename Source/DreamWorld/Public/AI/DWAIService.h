// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Base/AIServiceBase.h"
#include "DWAIService.generated.h"

/**
 * AI�������
 */
UCLASS()
class DREAMWORLD_API UDWAIService : public UAIServiceBase
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
