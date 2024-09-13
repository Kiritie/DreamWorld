// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWAIDecorator_CheckTarget.h"
#include "DWAIDecorator_CheckAttacking.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWAIDecorator_CheckAttacking : public UDWAIDecorator_CheckTarget
{
	GENERATED_UCLASS_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
