// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld.h"
#include "BehaviorTree/BlackboardData.h"
#include "DWAIBlackboard.generated.h"

/**
 * AI�ڰ����
 */
UCLASS()
class DREAMWORLD_API UDWAIBlackboard : public UBlackboardData
{
	GENERATED_BODY()
	
public:
	virtual void PostLoad() override;
};
