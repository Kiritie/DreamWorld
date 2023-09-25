// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/DWAIDecorator.h"
#include "DWAIDecorator_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWAIDecorator_CheckDistance : public UDWAIDecorator
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetCharacterKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetDistanceKey;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
