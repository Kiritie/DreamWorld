// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/DWAIService_QueryDefendPath.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIService_QueryDefendPath::UDWAIService_QueryDefendPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCallTickOnSearchStart = true;
	bRestartTimerOnEachActivation = true;

	DefendTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIService_QueryDefendPath, DefendTargetKey), ADWCharacter::StaticClass());
	DefendLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIService_QueryDefendPath, DefendLocationKey));
	
	DefendTarget = nullptr;
	DefendDistanceOffset = 0.f;
}

void UDWAIService_QueryDefendPath::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	DefendLocationKey.ResolveSelectedKey(*Asset.BlackboardAsset);
}

bool UDWAIService_QueryDefendPath::InitService(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitService(OwnerComp)) return false;

	DefendTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(DefendTargetKey.SelectedKeyName));

	return DefendTarget && DefendTarget->IsValidLowLevel();
}

void UDWAIService_QueryDefendPath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!InitService(OwnerComp)) return;

	RefreshDefendLocation(OwnerComp);
}

void UDWAIService_QueryDefendPath::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UDWAIService_QueryDefendPath::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!InitService(OwnerComp)) return;

	RefreshDefendLocation(OwnerComp);
}

void UDWAIService_QueryDefendPath::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	if (!InitService(OwnerComp)) return;
}

void UDWAIService_QueryDefendPath::RefreshDefendLocation(UBehaviorTreeComponent& OwnerComp)
{
	const float DefendDistance = DefendTarget->GetAttackDistance() + DefendDistanceOffset;
	
	const FVector DefendLocation = DefendTarget->GetActorLocation() + (GetAgent<ADWCharacter>()->GetDistance(DefendTarget, false, false) < DefendDistance ? (-GetAgent<ADWCharacter>()->GetActorForwardVector()) * DefendDistance : FVector::ZeroVector);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(DefendLocationKey.SelectedKeyName, DefendLocation);
}
