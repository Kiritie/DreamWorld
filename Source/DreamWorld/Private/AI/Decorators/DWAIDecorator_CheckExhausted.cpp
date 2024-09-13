// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckExhausted.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckExhausted::UDWAIDecorator_CheckExhausted(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckExhausted::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return CheckTarget->IsExhausted();
}
