// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/DWAITask_MoveTo.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Voxel/VoxelModuleStatics.h"

UDWAITask_MoveTo::UDWAITask_MoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MoveTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_MoveTo, MoveTargetKey), ADWCharacter::StaticClass());
	MoveTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_MoveTo, MoveTargetKey));

	MoveDistanceKey.AllowNoneAsValue(true);
	MoveDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_MoveTo, MoveDistanceKey));

	MovementRate = 1.f;
	RotationRate = 1.f;

	MoveTarget = nullptr;
	MoveLocation = FVector();
	MoveDistance = 0.f;
}

void UDWAITask_MoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	MoveTargetKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	MoveDistanceKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAITask_MoveTo::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	if (!Super::InitTask(OwnerComp)) return false;

	MoveDistance = MoveDistanceKey.SelectedKeyName.IsValid() ? OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MoveDistanceKey.SelectedKeyName) : 0.f;
	
	if (MoveTargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		MoveLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(MoveTargetKey.SelectedKeyName);
	}
	else
	{
		MoveTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MoveTargetKey.SelectedKeyName));
		return MoveTarget && MoveTarget->IsValidLowLevel();
	}
	return true;
}

void UDWAITask_MoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(!InitTask(OwnerComp)) return;

	if (MoveTargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		if(GetAgent<ADWCharacter>()->DoAIMove(MoveLocation, MoveDistance) && DurationTime == -1.f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if(MoveTarget->IsPlayer() && GetAgent<ADWCharacter>()->IsTeamMate(MoveTarget) && GetAgent<ADWCharacter>()->GetDistance(MoveTarget, false, false) >= MoveDistance * 10.f)
		{
			const FVector Direction = GetAgent<ADWCharacter>()->GetActorLocation() - MoveTarget->GetActorLocation();
			FVector RayStart = MoveTarget->GetActorLocation() + Direction.GetSafeNormal() * MoveDistance;
			RayStart.Z = UVoxelModuleStatics::GetWorldData().GetWorldRealSize().Z;
			const FVector RayEnd = FVector(RayStart.X, RayStart.Y, 0);

			FHitResult HitResult;
			UVoxelModuleStatics::VoxelAgentTraceSingle(RayStart, RayEnd, GetAgent<ADWCharacter>()->GetRadius(), GetAgent<ADWCharacter>()->GetHalfHeight(), {}, HitResult, false);
			if (HitResult.bBlockingHit)
			{
				GetAgent<ADWCharacter>()->SetActorLocation(HitResult.Location);
			}
			else
			{
				GetAgent<ADWCharacter>()->SetActorLocation(FVector(RayStart.X, RayStart.Y, MoveTarget->GetActorLocation().Z));
			}
		}
		if(GetAgent<ADWCharacter>()->DoAIMove(MoveTarget, MoveDistance) && DurationTime == -1.f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UDWAITask_MoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	//GetAgent<ADWCharacter>()->SetMotionRate(1.f, 1.f);

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	GetAgent<ADWCharacter>()->SetMotionRate(MovementRate, RotationRate);

	return EBTNodeResult::InProgress;
}

void UDWAITask_MoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (!InitTask(OwnerComp)) return;

	GetAgent<ADWCharacter>()->SetMotionRate(1.f, 1.f);
}
