// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckAttacking.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckAttacking::UDWAIDecorator_CheckAttacking(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return CheckTarget->IsAttacking();
}
