// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Slot/AbilityInventorySkillSlot.h"
#include "DWInventorySkillSlot.generated.h"

/**
 * ���ܲ�
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UDWInventorySkillSlot : public UAbilityInventorySkillSlot
{
	GENERATED_BODY()

public:
	UDWInventorySkillSlot();

public:
	virtual void OnInitialize(UAbilityInventoryBase* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESlotSplitType InSplitType /*= ESlotSplitType::Default*/) override;

	virtual void OnItemPreChange(FAbilityItem& InNewItem) override;

	virtual void OnItemChanged(FAbilityItem& InOldItem) override;
	
	virtual bool ActiveItem(bool bPassive = false) override;
	
	virtual void CancelItem(bool bPassive = false) override;

public:
	virtual FAbilityInfo GetAbilityInfo() const override;
	
	UFUNCTION(BlueprintPure)
	FDWCharacterSkillAbilityData GetSkillData() const;
};
