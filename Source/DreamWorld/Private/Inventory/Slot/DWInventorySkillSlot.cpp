// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/DWInventorySkillSlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
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

bool UDWInventorySkillSlot::MatchItemLimit(FAbilityItem InItem) const
{
	if(!Super::MatchItemLimit(InItem)) return false;

	ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();

	if(Character && Character->GetLevelA() < InItem.Level) return false;

	return true;
}

bool UDWInventorySkillSlot::ActiveItem(bool bPassive /*= false*/)
{
	if(IsEmpty()) return false;

	ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();

	if(!Character) return false;

	const auto AbilityData = Character->GetSkillAbility(Item.ID);
	if(Character->CheckWeaponType(AbilityData.WeaponType) && Super::ActiveItem(bPassive))
	{
		return Character->SkillAttack(Item);
	}
	return false;
}

void UDWInventorySkillSlot::DeactiveItem(bool bPassive /*= false*/)
{
	if(IsEmpty()) return;
	
	ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();

	if(!Character) return;

	const auto AbilityData = Character->GetSkillAbility(Item.ID);
	if(AbilityData.bCancelAble)
	{
		if(Character->GetSkillAbilityItem() == Item.ID)
		{
			Character->UnAttack();
		}
	}
	Super::DeactiveItem(bPassive);
}
