// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_AIDefend.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UDWAITask_AIDefend::UDWAITask_AIDefend(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefendTarget = nullptr;
	DefendTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIDefend, DefendTargetKey), ADWCharacter::StaticClass());
	AttackDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIDefend, AttackDistanceKey));

	AttackDistance = 0.f;
}

bool UDWAITask_AIDefend::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	DefendTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(DefendTargetKey.SelectedKeyName));
	AttackDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackDistanceKey.SelectedKeyName);

	return DefendTarget && DefendTarget->IsValidLowLevel();
}

void UDWAITask_AIDefend::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	if(GetAgent<ADWCharacter>()->IsDefending())
	{
		GetAgent<ADWCharacter>()->DoAIMove(GetAIMoveLocation());
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UDWAITask_AIDefend::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetAgent<ADWCharacter>()->UnDefend();
	//GetAgent<ADWCharacter>()->SetLockedTarget(nullptr);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_AIDefend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->Defend();
	GetAgent<ADWCharacter>()->SetLockedTarget(DefendTarget);

	return EBTNodeResult::InProgress;
}

void UDWAITask_AIDefend::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnDefend();
	GetAgent<ADWCharacter>()->SetLockedTarget(nullptr);
}

FVector UDWAITask_AIDefend::GetAIMoveLocation() const
{
	if(DefendTarget)
	{
		return DefendTarget->GetActorLocation() + (-GetAgent<ADWCharacter>()->GetActorForwardVector()) * (AttackDistance + 100.f);
	}
	return FVector::ZeroVector;
}
