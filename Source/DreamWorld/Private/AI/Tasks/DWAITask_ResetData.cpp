// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/DWAITask_ResetData.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/DWCharacter.h"
#include "AI/DWAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDWAITask_ResetData::UDWAITask_ResetData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = false;
	
	//TargetCharacterKey.AllowNoneAsValue(true);
	TargetCharacterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_ResetData, TargetCharacterKey), ADWCharacter::StaticClass());
	
	//TargetLocationKey.AllowNoneAsValue(true);
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_ResetData, TargetLocationKey));

	//IsLostTargetKey.AllowNoneAsValue(true);
	IsLostTargetKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UDWAITask_ResetData, IsLostTargetKey));
}

EBTNodeResult::Type UDWAITask_ResetData::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UDWAITask_ResetData::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitTask(OwnerComp)) return EBTNodeResult::Failed;

	if(!TargetCharacterKey.IsNone())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetCharacterKey.SelectedKeyName, nullptr);
	}
	if(!TargetLocationKey.IsNone())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, FVector::ZeroVector);
	}
	if(!IsLostTargetKey.IsNone())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLostTargetKey.SelectedKeyName, false);
	}
	
	return EBTNodeResult::Succeeded;
}
