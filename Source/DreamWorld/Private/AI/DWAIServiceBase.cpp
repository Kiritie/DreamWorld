// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIServiceBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"

UDWAIServiceBase::UDWAIServiceBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDWAIServiceBase::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitService(OwnerComp)) return;

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UDWAIServiceBase::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitService(OwnerComp)) return;

	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UDWAIServiceBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(!InitService(OwnerComp)) return;

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
