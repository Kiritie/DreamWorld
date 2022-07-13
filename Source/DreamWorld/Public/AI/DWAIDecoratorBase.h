// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "AI/Base/AIDecoratorBase.h"
#include "DWAIDecoratorBase.generated.h"

/**
 * AIװ��������
 */
UCLASS()
class DREAMWORLD_API UDWAIDecoratorBase : public UAIDecoratorBase
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
