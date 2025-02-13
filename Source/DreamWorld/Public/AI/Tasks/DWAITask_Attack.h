// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DWAITask_Fight.h"
#include "Common/DWCommonTypes.h"

#include "DWAITask_Attack.generated.h"

class ADWCharacter;

/**
 * AI任务_攻击
 */
UCLASS()
class DREAMWORLD_API UDWAITask_Attack : public UDWAITask_Fight
{
	GENERATED_BODY()
	
public:
	UDWAITask_Attack(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool InitTask(UBehaviorTreeComponent& OwnerComp) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int32 AttackAbilityIndex;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	EDWWeaponPart AttackWeaponPart;
};
