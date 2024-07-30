// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/AbilityModuleTypes.h"
#include "AI/Base/Tasks/AITask_Duration.h"
#include "DWAITask_SkillAttack.generated.h"

class ADWCharacter;

/**
 * AI任务_攻击
 */
UCLASS()
class DREAMWORLD_API UDWAITask_SkillAttack : public UAITask_Duration
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int32 SkillAbilityIndex;

private:
	UPROPERTY()
	ADWCharacter* AttackTarget;
};
