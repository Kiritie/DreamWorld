// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_AIPatrol.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "AI/DWAIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Global/GlobalTypes.h"
#include "Voxel/VoxelModuleBPLibrary.h"

UDWAITask_AIPatrol::UDWAITask_AIPatrol(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PatrolDistance = 0.f;
	PatrolLocation = FVector();

	PatrolDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIPatrol, PatrolDistanceKey));
	PatrolDurationKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIPatrol, PatrolDurationKey));
	PatrolLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_AIPatrol, PatrolLocationKey));
}

bool UDWAITask_AIPatrol::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if (!Super::InitTask(OwnerComp)) return false;

	PatrolLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(PatrolLocationKey.SelectedKeyName);

	return true;
}

void UDWAITask_AIPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitTask(OwnerComp)) return;

	if (GetOwnerCharacter<ADWCharacter>()->DoAIMove(PatrolLocation))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UDWAITask_AIPatrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetOwnerCharacter<ADWCharacter>()->SetMotionRate(1.f, 1.f);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_AIPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const float PatrolDuration= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(PatrolDurationKey.SelectedKeyName);
	DurationTime = FMath::RandRange(PatrolDuration - 2.f, PatrolDuration + 2.f);

	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetOwnerCharacter<ADWCharacter>()->SetMotionRate(0.3f, 0.5f);

	PatrolDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(PatrolDistanceKey.SelectedKeyName);

	PatrolLocation = GetOwnerCharacter<ADWCharacter>()->GetActorLocation();
	DON(i, 10,
		FVector rayStart = GetOwnerCharacter<ADWCharacter>()->GetBirthLocation() + FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f).Vector() * FMath::FRandRange(0.f, PatrolDistance);
		rayStart.Z = UVoxelModuleBPLibrary::GetWorldData().GetWorldHeight(true);
		const FVector rayEnd = FVector(rayStart.X, rayStart.Y, 0.f);
		FHitResult hitResult;
		if(UVoxelModuleBPLibrary::VoxelAgentTraceSingle(rayStart, rayEnd, GetOwnerCharacter<ADWCharacter>()->GetRadius(), GetOwnerCharacter<ADWCharacter>()->GetHalfHeight(), {}, hitResult))
		{
			PatrolLocation = hitResult.Location;
			break;
		}
	)
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, PatrolLocation);

	return EBTNodeResult::InProgress;
}

void UDWAITask_AIPatrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetOwnerCharacter<ADWCharacter>()->SetMotionRate(1.f, 1.f);
}
