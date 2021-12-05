// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "BehaviorTree/BTService.h"
#include "DWAIServiceBase.generated.h"

class ADWCharacter;
class ADWAIController;

/**
 * AI�������
 */
UCLASS()
class DREAMWORLD_API UDWAIServiceBase : public UBTService
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(Transient)
	ADWAIController* OwnerController;

	UPROPERTY(Transient)
	ADWCharacter* OwnerCharacter;

protected:
	virtual bool InitService(UBehaviorTreeComponent& OwnerComp);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
