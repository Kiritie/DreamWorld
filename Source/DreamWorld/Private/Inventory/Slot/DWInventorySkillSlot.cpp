// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/DWInventorySkillSlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"

UDWInventorySkillSlot::UDWInventorySkillSlot()
{
}

void UDWInventorySkillSlot::OnInitialize(UAbilityInventoryBase* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType);
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
		return Character->SkillAttackImpl(Item.ID);
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
			if(Character->GetSkillAbilityID() == Item.ID)
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
