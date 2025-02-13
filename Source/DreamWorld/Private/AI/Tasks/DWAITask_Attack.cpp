// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_Attack.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"

UDWAITask_Attack::UDWAITask_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttackAbilityIndex = -1;
	AttackWeaponPart = EDWWeaponPart::None;
}

void UDWAITask_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}

bool UDWAITask_Attack::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	return Super::InitTask(OwnerComp);
}

void UDWAITask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;
}

EBTNodeResult::Type UDWAITask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->UnAttack();

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	if(GetAgent<ADWCharacter>()->Attack(AttackWeaponPart, AttackAbilityIndex, FSimpleDelegate::CreateLambda([this, &OwnerComp]()
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	})))
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UDWAITask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnAttack();
}
