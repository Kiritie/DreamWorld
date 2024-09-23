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

	if(ADWHumanCharacter* HumanCharacter = GetAgent<ADWHumanCharacter>())
	{
		return HumanCharacter->GetNature() == EDWCharacterNature::NPC && CheckTarget->IsPlayer();
	}
	else if(ADWMonsterCharacter* MonsterCharacter = GetAgent<ADWMonsterCharacter>())
	{
		const FAbilityItem SelectedItem = CheckTarget->GetInventory()->GetSelectedItem();
		return SelectedItem.IsValid() && SelectedItem.GetType() == EAbilityItemType::Prop && SelectedItem.GetData<UDWPropData>().PropType == EDWPropType::Food;
	}
	return false;
}
