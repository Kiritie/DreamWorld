// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckDefending.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DWCharacter.h"

UDWAIDecorator_CheckDefending::UDWAIDecorator_CheckDefending(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = false;
	
	TargetCharacterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckDefending, TargetCharacterKey), ADWCharacter::StaticClass());
}

bool UDWAIDecorator_CheckDefending::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(ADWCharacter* TargetCharacter = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetCharacterKey.SelectedKeyName)))
	{
		return TargetCharacter->IsDefending();
	}
	return false;
}
