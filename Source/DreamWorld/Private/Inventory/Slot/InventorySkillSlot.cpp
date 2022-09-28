// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventorySkillSlot.h"

#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetItemInfoBox.h"
#include "Ability/Character/DWCharacterSkillAbility.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/Abilities/ItemAbilityBase.h"

UInventorySkillSlot::UInventorySkillSlot()
{
}

void UInventorySkillSlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType);
}

void UInventorySkillSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
	if(Item.GetData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		CancelItem();
	}
}

void UInventorySkillSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
	if(Item.GetData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		ActiveItem();
	}
}

bool UInventorySkillSlot::ActiveItem()
{
	if(!Super::ActiveItem()) return false;
	
	if(ADWCharacter* Character = Cast<ADWCharacter>(Inventory->GetOwnerActor()))
	{
		return Character->SkillAttack(Item.ID);
	}
	return false;
}

bool UInventorySkillSlot::CancelItem()
{
	if(IsEmpty()) return false;
	
	if (GetSkillData().bCancelable)
	{
		return Super::CancelItem();
	}
	return false;
}

FAbilityInfo UInventorySkillSlot::GetAbilityInfo() const
{
	return Super::GetAbilityInfo();
}

FDWCharacterSkillAbilityData UInventorySkillSlot::GetSkillData() const
{
	if(ADWCharacter* Character = Cast<ADWCharacter>(Inventory->GetOwnerActor()))
	{
		return Character->GetSkillAbility(Item.ID);
	}
	return FDWCharacterSkillAbilityData();
}
