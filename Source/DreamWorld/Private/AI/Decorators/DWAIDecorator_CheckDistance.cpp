// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckDistance.h"

#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Debug/DebugTypes.h"

UDWAIDecorator_CheckDistance::UDWAIDecorator_CheckDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckDistance, CheckDistanceKey));
}

bool UDWAIDecorator_CheckDistance::InitDecorator(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitDecorator(OwnerComp)) return false;

	CheckDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(CheckDistanceKey.SelectedKeyName);
	
	return true;
}

bool UDWAIDecorator_CheckDistance::InitDecorator(UBehaviorTreeComponent& OwnerComp) const
{
	return Super::InitDecorator(OwnerComp);
}

bool UDWAIDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;

	return GetAgent<ADWCharacter>()->GetDistance(CheckTarget, false, false) <= CheckDistance;
}
