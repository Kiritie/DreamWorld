// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_SkillAttack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/Looking/LookingComponent.h"

UDWAITask_SkillAttack::UDWAITask_SkillAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttackTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_SkillAttack, AttackTargetKey), ADWCharacter::StaticClass());

	AttackTarget = nullptr;
	SkillType = ESkillType::None;
	SkillAbilityIndex = -1;
}

void UDWAITask_SkillAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	AttackTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	AttackTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_SkillAttack::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	AttackTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));

	return AttackTarget && AttackTarget->IsValidLowLevel();
}

void UDWAITask_SkillAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->SkillAttack(SkillType, SkillAbilityIndex);
}

EBTNodeResult::Type UDWAITask_SkillAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_SkillAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOn(AttackTarget);

	return EBTNodeResult::InProgress;
}

void UDWAITask_SkillAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnAttack();
	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOff();
}
