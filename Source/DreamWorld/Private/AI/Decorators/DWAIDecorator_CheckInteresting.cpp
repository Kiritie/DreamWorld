// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/DWAIDecorator_CheckInteresting.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/DWCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Character/Monster/DWMonsterCharacter.h"
#include "Item/Prop/DWPropData.h"

UDWAIDecorator_CheckInteresting::UDWAIDecorator_CheckInteresting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDWAIDecorator_CheckInteresting::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!InitDecorator(OwnerComp)) return false;

	const ADWCharacter* Character = GetAgent<ADWCharacter>();
	if(Character->IsA<ADWHumanCharacter>())
	{
		return CheckTarget->IsPlayer();
	}
	else if(Character->IsA<ADWMonsterCharacter>())
	{
		const FAbilityItem SelectedItem = CheckTarget->GetInventory()->GetSelectedItem(ESlotSplitType::Shortcut);
		return SelectedItem.IsValid() && SelectedItem.IsDataType<UDWPropData>() && SelectedItem.GetData<UDWPropData>(false).PropType == EDWPropType::Food;
	}
	return false;
}
