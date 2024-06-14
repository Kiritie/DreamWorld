// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckInteresting.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/DWMonsterCharacter.h"
#include "Item/Prop/DWPropData.h"

UDWAIDecorator_CheckInteresting::UDWAIDecorator_CheckInteresting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDWAIDecorator_CheckInteresting, CheckTargetKey), ADWCharacter::StaticClass());
}

bool UDWAIDecorator_CheckInteresting::InitDecorator(UBehaviorTreeComponent& OwnerComp)
{
	if(!Super::InitDecorator(OwnerComp)) return false;

	CheckTarget = Cast<ADWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CheckTargetKey.SelectedKeyName));
	
	return CheckTarget && CheckTarget->IsValidLowLevel();
}

bool UDWAIDecorator_CheckInteresting::InitDecorator(UBehaviorTreeComponent& OwnerComp) const
{
	return Super::InitDecorator(OwnerComp);
}

bool UDWAIDecorator_CheckInteresting::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;

	if(GetAgent<ADWMonsterCharacter>())
	{
		const FAbilityItem SelectedItem = CheckTarget->GetInventory()->GetSelectedItem();
		return SelectedItem.IsValid() && SelectedItem.GetType() == EAbilityItemType::Prop && SelectedItem.GetData<UDWPropData>().PropType == EDWPropType::Food;
	}
	return false;
}
