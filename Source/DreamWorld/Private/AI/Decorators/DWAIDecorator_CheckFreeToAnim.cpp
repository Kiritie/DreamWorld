// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckFreeToAnim.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckFreeToAnim::UDWAIDecorator_CheckFreeToAnim(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckFreeToAnim::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return CheckTarget->IsFreeToAnim();
}
