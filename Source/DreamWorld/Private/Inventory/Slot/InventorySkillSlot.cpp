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

UInventorySkillSlot::UInventorySkillSlot()
{
	LimitType = EAbilityItemType::Skill;
}

void UInventorySkillSlot::InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::InitSlot(InOwner, InItem, InLimitType, InSplitType);
}

void UInventorySkillSlot::PreSet(FAbilityItem& InItem)
{
	Super::PreSet(InItem);
	if(InItem.IsValid() && GetSkillData().GetItemData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		CancelItem();
	}
}

void UInventorySkillSlot::EndSet()
{
	Super::EndSet();
	if(Item.IsValid() &&  GetSkillData().GetItemData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Passive)
	{
		ActiveItem();
	}
}

bool UInventorySkillSlot::ActiveItem()
{
	if(ADWCharacter* Character = Cast<ADWCharacter>(Owner->GetOwnerActor()))
	{
		if (Character->SkillAttack(Item.ID))
		{
			Super::ActiveItem();
			if(GetSkillData().GetItemData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Initiative)
			{
				StartCooldown();
			}
			return true;
		}
		else if(CooldownInfo.bCooldowning)
		{
			TArray<FParameter> Params { FParameter::MakeString(TEXT("该技能处于冷却中！")) };
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetItemInfoBox>(&Params);
		}
	}
	return false;
}

bool UInventorySkillSlot::CancelItem()
{
	if (GetSkillData().bCancelable)
	{
		Super::CancelItem();
		if(GetSkillData().GetItemData<UAbilitySkillDataBase>().SkillMode == ESkillMode::Initiative)
		{
			StopCooldown();
		}
		return true;
	}
	return false;
}

FAbilityInfo UInventorySkillSlot::GetAbilityInfo() const
{
	FAbilityInfo AbilityInfo;
	if(ADWCharacter* Character = Cast<ADWCharacter>(Owner->GetOwnerActor()))
	{
		Character->GetAbilityInfo(GetSkillData().AbilityClass, AbilityInfo);
	}
	return AbilityInfo;
}

FDWCharacterSkillAbilityData UInventorySkillSlot::GetSkillData() const
{
	if(ADWCharacter* Character = Cast<ADWCharacter>(Owner->GetOwnerActor()))
	{
		return Character->GetSkillAbility(Item.ID);
	}
	return FDWCharacterSkillAbilityData();
}
