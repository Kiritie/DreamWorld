// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWAIDecorator_CheckTarget.h"
#include "DWAIDecorator_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWAIDecorator_CheckDistance : public UDWAIDecorator_CheckTarget
{
	GENERATED_BODY()

public:
	UDWAIDecorator_CheckDistance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool InitDecorator(UBehaviorTreeComponent& OwnerComp) override;

	virtual bool InitDecorator(UBehaviorTreeComponent& OwnerComp) const override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CheckDistanceKey;

private:
	float CheckDistance;
};
