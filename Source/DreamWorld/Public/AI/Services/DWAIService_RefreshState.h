// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "AI/DWAIServiceBase.h"
#include "DWAIService_RefreshState.generated.h"

class ADWCharacter;

/**
 * AI����_ˢ��״̬
 */
UCLASS()
class DREAMWORLD_API UDWAIService_RefreshState : public UDWAIServiceBase
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CharacterAIStateKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetCharacterKey;
		
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsLostTargetKey;

private:
	EDWCharacterAIState CharacterAIState;

	ADWCharacter* TargetCharacter;

	bool IsLostTarget;

protected:
	virtual bool InitService(UBehaviorTreeComponent& OwnerComp) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void SetCharacterAIState(UBehaviorTreeComponent& OwnerComp, EDWCharacterAIState InAIState);

	void SetTargetCharacter(UBehaviorTreeComponent& OwnerComp, ADWCharacter* InTargetCharacter);

	void SetIsLostTarget(UBehaviorTreeComponent& OwnerComp, bool InIsLostTarget);
};
