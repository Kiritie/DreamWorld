// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIDecorator.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"

UDWAIDecorator::UDWAIDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

bool UDWAIDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
