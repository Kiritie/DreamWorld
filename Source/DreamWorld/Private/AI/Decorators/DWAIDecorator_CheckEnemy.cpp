// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckEnemy.h"

#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UDWAIDecorator_CheckEnemy::UDWAIDecorator_CheckEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return GetAgent<ADWCharacter>()->IsEnemy(CheckTarget);
}
