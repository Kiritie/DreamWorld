// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIService.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"

UDWAIService::UDWAIService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDWAIService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitService(OwnerComp)) return;

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UDWAIService::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitService(OwnerComp)) return;

	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UDWAIService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(!InitService(OwnerComp)) return;

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
