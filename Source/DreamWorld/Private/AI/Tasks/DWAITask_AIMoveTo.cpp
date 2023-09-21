// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_AIMoveTo.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BehaviorTree.h"

UDWAITask_AIMoveTo::UDWAITask_AIMoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIMoveTo, TargetKey), ADWCharacter::StaticClass());
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIMoveTo, TargetKey));
	DistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIMoveTo, DistanceKey));

	TargetCharacter = nullptr;
	TargetLocation = FVector();
	TargetDistance = 0.f;
}

void UDWAITask_AIMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	TargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	DistanceKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_AIMoveTo::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if (!Super::InitTask(OwnerComp)) return false;

	TargetDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DistanceKey.SelectedKeyName);
	if (TargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetKey.SelectedKeyName);
	}
	else
	{
		TargetCharacter = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		return TargetCharacter && TargetCharacter->IsValidLowLevel();
	}
	return true;
}

void UDWAITask_AIMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(!InitTask(OwnerComp)) return;

	if (TargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		if(GetOwnerCharacter<ADWCharacter>()->DoAIMove(TargetLocation, TargetDistance))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if(GetOwnerCharacter<ADWCharacter>()->DoAIMove(TargetCharacter, TargetDistance))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UDWAITask_AIMoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetOwnerCharacter<ADWCharacter>()->SetMotionRate(1.f, 1.f);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_AIMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetOwnerCharacter<ADWCharacter>()->SetMotionRate(0.7f, 0.7f);
	
	return EBTNodeResult::InProgress;
}

void UDWAITask_AIMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetOwnerCharacter<ADWCharacter>()->SetMotionRate(1.f, 1.f);
}
