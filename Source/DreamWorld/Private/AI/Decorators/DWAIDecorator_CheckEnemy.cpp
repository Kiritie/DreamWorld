// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckEnemy.h"

#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UDWAIDecorator_CheckEnemy::UDWAIDecorator_CheckEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckEnemy, CheckTargetKey), ADWCharacter::StaticClass());
}

bool UDWAIDecorator_CheckEnemy::InitDecorator(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitDecorator(OwnerComp)) return false;

	CheckTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CheckTargetKey.SelectedKeyName));
	
	return CheckTarget && CheckTarget->IsValidLowLevel();
}

bool UDWAIDecorator_CheckEnemy::InitDecorator(UBehaviorTreeComponent& OwnerComp) const
{
	return Super::InitDecorator(OwnerComp);
}

bool UDWAIDecorator_CheckEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;
	
	return GetAgent<ADWCharacter>()->IsEnemy(CheckTarget);
}
