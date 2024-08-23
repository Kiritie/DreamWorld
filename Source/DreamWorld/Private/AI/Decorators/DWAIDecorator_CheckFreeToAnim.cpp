// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckFreeToAnim.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckFreeToAnim::UDWAIDecorator_CheckFreeToAnim(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckFreeToAnim, CheckTargetKey), ADWCharacter::StaticClass());
}

bool UDWAIDecorator_CheckFreeToAnim::InitDecorator(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitDecorator(OwnerComp)) return false;

	CheckTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CheckTargetKey.SelectedKeyName));
	
	return CheckTarget && CheckTarget->IsValidLowLevel();
}

bool UDWAIDecorator_CheckFreeToAnim::InitDecorator(UBehaviorTreeComponent& OwnerComp) const
{
	return Super::InitDecorator(OwnerComp);
}

bool UDWAIDecorator_CheckFreeToAnim::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return GetAgent<ADWCharacter>()->IsFreeToAnim();
}
