// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWAIDecorator_CheckTarget.h"
#include "DWAIDecorator_CheckExhausted.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWAIDecorator_CheckExhausted : public UDWAIDecorator_CheckTarget
{
	GENERATED_BODY()

public:
	UDWAIDecorator_CheckExhausted(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
