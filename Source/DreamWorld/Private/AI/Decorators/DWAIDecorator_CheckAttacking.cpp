// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckAttacking.h"

#include "Character/DWCharacter.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AI/DWAIController.h"

UDWAIDecorator_CheckAttacking::UDWAIDecorator_CheckAttacking(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = false;
	
	TargetCharacterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckAttacking, TargetCharacterKey), ADWCharacter::StaticClass());
}

bool UDWAIDecorator_CheckAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(ADWCharacter* TargetCharacter = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetCharacterKey.SelectedKeyName)))
	{
		return TargetCharacter->IsAttacking();
	}
	return false;
}
