// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/DWAIService_QueryPatrolPath.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Scene/SceneModuleStatics.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"

UDWAIService_QueryPatrolPath::UDWAIService_QueryPatrolPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCallTickOnSearchStart = true;
	bRestartTimerOnEachActivation = true;

	Interval = -1.f;

	PatrolDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIService_QueryPatrolPath, PatrolDistanceKey));
	PatrolDurationKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIService_QueryPatrolPath, PatrolDurationKey));
	PatrolLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIService_QueryPatrolPath, PatrolLocationKey));
}

void UDWAIService_QueryPatrolPath::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	PatrolDistanceKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	PatrolDurationKey.ResolveSelectedKey(*Asset.BlackboardAsset);
	PatrolLocationKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAIService_QueryPatrolPath::InitService(UBehaviorTreeComponent& OwnerComp)
{
	return Super::InitService(OwnerComp);
}

void UDWAIService_QueryPatrolPath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitService(OwnerComp)) return;

	const float PatrolDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(PatrolDistanceKey.SelectedKeyName);

	FVector PatrolLocation = GetAgent<ADWCharacter>()->GetActorLocation();
	DON(10,
		FVector rayStart = GetAgent<ADWCharacter>()->GetBirthTransform().GetLocation() + FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f).Vector() * FMath::FRandRange(0.f, PatrolDistance);
		const FVector rayEnd = FVector(rayStart.X, rayStart.Y, 0.f);
		FHitResult hitResult;
		if(UVoxelModule::IsValid())
		{
			rayStart.Z = UVoxelModuleStatics::GetWorldData().GetWorldRealSize().Z;
			UVoxelModuleStatics::VoxelAgentTraceSingle(rayStart, rayEnd, GetAgent<ADWCharacter>()->GetRadius(), GetAgent<ADWCharacter>()->GetHalfHeight(), {}, hitResult);
		}
		else
		{
			rayStart.Z = 10000.f;
			UKismetSystemLibrary::CapsuleTraceSingle(GetWorldContext(), rayStart, rayEnd, GetAgent<ADWCharacter>()->GetRadius(), GetAgent<ADWCharacter>()->GetHalfHeight(), USceneModuleStatics::GetTraceMapping(FName("Chunk")).GetTraceType(), false, {}, EDrawDebugTrace::None, hitResult, true);
		}
		if(hitResult.bBlockingHit)
		{
			PatrolLocation = hitResult.Location;
			break;
		}
	)
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, PatrolLocation);
}

void UDWAIService_QueryPatrolPath::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UDWAIService_QueryPatrolPath::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!InitService(OwnerComp)) return;

	Interval = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(PatrolDurationKey.SelectedKeyName);
}

void UDWAIService_QueryPatrolPath::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	if (!InitService(OwnerComp)) return;
}
