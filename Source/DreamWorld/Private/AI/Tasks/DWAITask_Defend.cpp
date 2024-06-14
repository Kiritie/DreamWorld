// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_Defend.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"
#include "Common/Looking/LookingComponent.h"

UDWAITask_Defend::UDWAITask_Defend(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefendTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_Defend, DefendTargetKey), ADWCharacter::StaticClass());
	DefendDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_Defend, DefendDistanceKey));

	DefendTarget = nullptr;
	DefendDistance = 0.f;
}

void UDWAITask_Defend::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	DefendTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	DefendDistanceKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_Defend::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	DefendTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(DefendTargetKey.SelectedKeyName));
	DefendDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DefendDistanceKey.SelectedKeyName);

	return DefendTarget && DefendTarget->IsValidLowLevel();
}

void UDWAITask_Defend::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

EBTNodeResult::Type UDWAITask_Defend::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetAgent<ADWCharacter>()->UnDefend();
	//GetAgent<ADWCharacter>()->SetLockedTarget(nullptr);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_Defend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->Defend();
	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOn(DefendTarget);

	return EBTNodeResult::InProgress;
}

void UDWAITask_Defend::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->UnDefend();
	GetAgent<ADWCharacter>()->GetLooking()->TargetLookingOff();
}

FVector UDWAITask_Defend::GetAIMoveLocation() const
{
	if(DefendTarget)
	{
		return DefendTarget->GetActorLocation() + (-GetAgent<ADWCharacter>()->GetActorForwardVector()) * (DefendDistance + 100.f);
	}
	return FVector::ZeroVector;
}
