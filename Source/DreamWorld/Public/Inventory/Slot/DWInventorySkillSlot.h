// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Slot/AbilityInventorySkillSlotBase.h"
#include "Common/DWCommonTypes.h"
#include "DWInventorySkillSlot.generated.h"

/**
 * ���ܲ�
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UDWInventorySkillSlot : public UAbilityInventorySkillSlotBase
{
	GENERATED_BODY()

public:
	UDWInventorySkillSlot();

public:
	virtual void OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex) override;

	virtual void OnItemPreChange(FAbilityItem& InNewItem, bool bBroadcast) override;

	virtual void OnItemChanged(FAbilityItem& InOldItem, bool bBroadcast) override;

	virtual bool MatchItemLimit(FAbilityItem InItem, bool bForce = false) const override;
	
	virtual bool ActiveItem(bool bPassive = false) override;
	
	virtual void DeactiveItem(bool bPassive = false) override;
};
