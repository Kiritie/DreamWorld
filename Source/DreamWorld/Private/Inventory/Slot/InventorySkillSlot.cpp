// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventorySkillSlot.h"

#include "Ability/Item/Skill/DWSkillAsset.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetItemInfoBox.h"
#include "Ability/Character/DWCharacterSkillAbility.h"

UInventorySkillSlot::UInventorySkillSlot()
{
	LimitType = EItemType::Skill;
}

void UInventorySkillSlot::InitSlot(UInventory* InOwner, FItem InItem, EItemType InLimitType /* = EItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::InitSlot(InOwner, InItem, InLimitType, InSplitType);
}

void UInventorySkillSlot::PreSet(FItem& InItem)
{
	Super::PreSet(InItem);
	if(GetSkillData().GetItemData<UDWSkillAsset>()->SkillMode == ESkillMode::Passive)
	{
		CancelItem();
	}
}

void UInventorySkillSlot::EndSet()
{
	Super::EndSet();
	if(GetSkillData().GetItemData<UDWSkillAsset>()->SkillMode == ESkillMode::Passive)
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
			if(GetSkillData().GetItemData<UDWSkillAsset>()->SkillMode == ESkillMode::Initiative)
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
		if(GetSkillData().GetItemData<UDWSkillAsset>()->SkillMode == ESkillMode::Initiative)
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
