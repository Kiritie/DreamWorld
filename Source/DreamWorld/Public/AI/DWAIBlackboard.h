// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Base/AIBlackboardBase.h"
#include "DWAIBlackboard.generated.h"

/**
 * AI黑板基类
 */
UCLASS()
class DREAMWORLD_API UDWAIBlackboard : public UAIBlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLoad() override;

	virtual void Initialize(UBlackboardComponent* InComponent) override;

protected:
	virtual void OnReset() override;
	
	virtual void OnRefresh() override;

	virtual void OnValueReset(FName InValueName) override;

	virtual void OnValuePreChange(FName InValueName) override;

	virtual void OnValueChanged(FName InValueName) override;

public:
	BLACKBOARD_VALUE_ACCESSORS_BOOL(IsExcessived);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(AttackDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(FollowDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(PatrolDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(PatrolDuration);
};
