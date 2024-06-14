// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/AIDecoratorBase.h"
#include "DWAIDecorator_CheckInteresting.generated.h"

class ADWCharacter;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWAIDecorator_CheckInteresting : public UAIDecoratorBase
{
	GENERATED_UCLASS_BODY()

protected:
	virtual bool InitDecorator(UBehaviorTreeComponent& OwnerComp) override;

	virtual bool InitDecorator(UBehaviorTreeComponent& OwnerComp) const override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CheckTargetKey;

private:
	UPROPERTY()
	ADWCharacter* CheckTarget;
};
