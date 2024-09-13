// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckDefending.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckDefending::UDWAIDecorator_CheckDefending(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckDefending::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return CheckTarget->IsDefending();
}
