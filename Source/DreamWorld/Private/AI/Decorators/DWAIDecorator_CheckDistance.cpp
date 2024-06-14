// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckDistance.h"

#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Debug/DebugTypes.h"

UDWAIDecorator_CheckDistance::UDWAIDecorator_CheckDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckDistance, CheckTargetKey), ADWCharacter::StaticClass());
	CheckDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckDistance, CheckDistanceKey));
}

bool UDWAIDecorator_CheckDistance::InitDecorator(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitDecorator(OwnerComp)) return false;

	CheckTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CheckTargetKey.SelectedKeyName));
	CheckDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(CheckDistanceKey.SelectedKeyName);
	
	return CheckTarget && CheckTarget->IsValidLowLevel();
}

bool UDWAIDecorator_CheckDistance::InitDecorator(UBehaviorTreeComponent& OwnerComp) const
{
	return Super::InitDecorator(OwnerComp);
}

bool UDWAIDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;

	WHDebug(FString::Printf(TEXT("%f__%f"), GetAgent<ADWCharacter>()->GetDistance(CheckTarget, false, false), CheckDistance));
	
	return GetAgent<ADWCharacter>()->GetDistance(CheckTarget, false, false) <= CheckDistance;
}
