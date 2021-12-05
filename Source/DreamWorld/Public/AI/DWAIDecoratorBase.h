// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "BehaviorTree/BTDecorator.h"
#include "DWAIDecoratorBase.generated.h"

class ADWAIController;
class ADWCharacter;

/**
 * AIװ��������
 */
UCLASS()
class DREAMWORLD_API UDWAIDecoratorBase : public UBTDecorator
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
