// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "Ability/Inventory/Slot/AbilityInventoryEquipSlot.h"
#include "DWInventoryEquipSlot.generated.h"

/**
 * װ����
 */
UCLASS()
class DREAMWORLD_API UDWInventoryEquipSlot : public UAbilityInventoryEquipSlot
{
	GENERATED_BODY()

public:
	UDWInventoryEquipSlot();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDWEquipPartType PartType;

public:
	virtual void OnInitialize(UAbilityInventoryBase* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType, ESplitSlotType InSplitType) override;

	virtual void OnInitialize(UAbilityInventoryBase* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType, ESplitSlotType InSplitType, int32 InPartType) override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual bool CheckSlot(FAbilityItem& InItem) const override;
	
	virtual void Refresh() override;

	virtual void OnItemPreChange(FAbilityItem& InNewItem) override;

	virtual void OnItemChanged(FAbilityItem& InOldItem) override;

public:
	EDWEquipPartType GetPartType() const { return PartType; }
};
