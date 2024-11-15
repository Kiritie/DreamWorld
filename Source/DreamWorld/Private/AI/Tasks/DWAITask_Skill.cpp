// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_Skill.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"

UDWAITask_Skill::UDWAITask_Skill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillType = EDWSkillType::None;
	SkillAbilityIndex = -1;
}

void UDWAITask_Skill::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}

bool UDWAITask_Skill::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	return Super::InitTask(OwnerComp);
}

void UDWAITask_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UDWAITask_Skill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->Skill(SkillType, SkillAbilityIndex);

	return EBTNodeResult::InProgress;
}

void UDWAITask_Skill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnAttack();
}
