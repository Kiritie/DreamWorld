// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
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

	virtual void Initialize(UBlackboardComponent* InComponent, ACharacterBase* InCharacter) override;

protected:
	virtual void OnRefresh() override;

	virtual void OnValuePreChange(FName InValueName) override;

	virtual void OnValueChanged(FName InValueName) override;

public:
	BLACKBOARD_VALUE_ACCESSORS_ENUM(CharacterNature);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(AttackDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(FollowDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(PatrolDistance);
	BLACKBOARD_VALUE_ACCESSORS_FLOAT(PatrolDuration);
};
