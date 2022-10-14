// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_AIFollow.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "AI/DWAIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UDWAITask_AIFollow::UDWAITask_AIFollow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TargetCharacter = nullptr;
	TargetDistance = 0.f;

	TargetCharacterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIFollow, TargetCharacterKey), ADWCharacter::StaticClass());
	TargetDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIFollow, TargetDistanceKey));
}

bool UDWAITask_AIFollow::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitTask(OwnerComp)) return false;

	TargetCharacter = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetCharacterKey.SelectedKeyName));
	TargetDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);
	return TargetCharacter && TargetCharacter->IsValidLowLevel();
}

void UDWAITask_AIFollow::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	GetOwnerCharacter<ADWCharacter>()->DoAIMove(TargetCharacter, TargetDistance, true);
}

EBTNodeResult::Type UDWAITask_AIFollow::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_AIFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}
