// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "Ability/Inventory/Slot/AbilityInventoryEquipSlotBase.h"
#include "DWInventoryEquipSlot.generated.h"

/**
 * װ����
 */
UCLASS()
class DREAMWORLD_API UDWInventoryEquipSlot : public UAbilityInventoryEquipSlotBase
{
	GENERATED_BODY()

public:
	UDWInventoryEquipSlot();

public:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex) override;

	virtual void OnItemPreChange(FAbilityItem& InNewItem) override;

	virtual void OnItemChanged(FAbilityItem& InOldItem) override;

	virtual bool MatchItemLimit(FAbilityItem InItem, bool bForce = false) const override;
	
	virtual void Refresh() override;

	virtual bool ActiveItem(bool bPassive) override;

	virtual void DeactiveItem(bool bPassive) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDWEquipPart EquipPart;

public:
	EDWEquipPart GetEquipPart() const { return EquipPart; }
};
