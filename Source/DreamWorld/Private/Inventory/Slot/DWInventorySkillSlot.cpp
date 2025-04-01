// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/DWInventorySkillSlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Item/Skill/DWSkillData.h"

UDWInventorySkillSlot::UDWInventorySkillSlot()
{
}

void UDWInventorySkillSlot::OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex)
{
	Super::OnInitialize(InInventory, InLimitType, InSplitType, InSlotIndex);
}

void UDWInventorySkillSlot::OnItemPreChange(FAbilityItem& InNewItem, bool bBroadcast)
{
	Super::OnItemPreChange(InNewItem, bBroadcast);
}

void UDWInventorySkillSlot::OnItemChanged(FAbilityItem& InOldItem, bool bBroadcast)
{
	Super::OnItemChanged(InOldItem, bBroadcast);
}

bool UDWInventorySkillSlot::MatchItemLimit(FAbilityItem InItem, bool bForce) const
{
	return Super::MatchItemLimit(InItem, bForce);
}

bool UDWInventorySkillSlot::ActiveItem(bool bPassive /*= false*/)
{
	if(IsEmpty()) return false;
	
	if(!bPassive)
	{
		ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();

		if(!Character || Character->GetControlMode() != EDWCharacterControlMode::Fighting) return false;

		const auto& SkillData = Item.GetData<UDWSkillData>();
		if(Character->CheckWeaponType(SkillData.WeaponPart, SkillData.WeaponType))
		{
			if(Super::ActiveItem(bPassive))
			{
				if(SkillData.SkillType == EDWSkillType::Attack || SkillData.SkillType == EDWSkillType::Magic)
				{
					if(!Character->SkillAttack(Item))
					{
						DeactiveItem(bPassive);
						return false;
					}
				}
				return true;
			}
		}
	}
	else
	{
		return Super::ActiveItem(bPassive);
	}
	return false;
}

void UDWInventorySkillSlot::DeactiveItem(bool bPassive /*= false*/)
{
	if(IsEmpty()) return;

	Super::DeactiveItem(bPassive);

	if(!bPassive)
	{
		ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();

		if(!Character) return;

		if(Item.GetData<UDWSkillData>().bCancelAble)
		{
			if(Character->GetSkillAttackAbilityItem() == Item)
			{
				Character->UnAttack();
			}
		}
	}
}
