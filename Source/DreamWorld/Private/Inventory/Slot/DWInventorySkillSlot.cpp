// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/DWInventorySkillSlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"

UDWInventorySkillSlot::UDWInventorySkillSlot()
{
}

void UDWInventorySkillSlot::OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex)
{
	Super::OnInitialize(InInventory, InLimitType, InSplitType, InSlotIndex);
}

void UDWInventorySkillSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
}

void UDWInventorySkillSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
}

bool UDWInventorySkillSlot::ActiveItem(bool bPassive /*= false*/)
{
	if(!Super::ActiveItem(bPassive)) return false;
	
	if(ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>())
	{
		return Character->SkillAttack(Item);
	}
	return false;
}

void UDWInventorySkillSlot::CancelItem(bool bPassive /*= false*/)
{
	if(IsEmpty()) return;
	
	if(GetSkillData().bCancelAble)
	{
		if(ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>())
		{
			if(Character->GetSkillAbilityItem() == Item.ID)
			{
				Character->UnAttack();
			}
		}
		Super::CancelItem(bPassive);
	}
}

FAbilityInfo UDWInventorySkillSlot::GetAbilityInfo() const
{
	return Super::GetAbilityInfo();
}

FDWCharacterSkillAbilityData UDWInventorySkillSlot::GetSkillData() const
{
	if(ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>())
	{
		return Character->GetSkillAbility(Item.ID);
	}
	return FDWCharacterSkillAbilityData();
}
