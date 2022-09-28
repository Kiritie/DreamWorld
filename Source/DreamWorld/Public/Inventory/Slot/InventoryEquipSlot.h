// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Inventory/Slot/InventorySlot.h"
#include "InventoryEquipSlot.generated.h"

/**
 * װ����
 */
UCLASS()
class DREAMWORLD_API UInventoryEquipSlot : public UInventorySlot
{
	GENERATED_BODY()

public:
	UInventoryEquipSlot();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	EDWEquipPartType PartType;

public:
	virtual void OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/) override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

	virtual bool CheckSlot(FAbilityItem& InItem) const override;
	
	virtual void Refresh() override;

	virtual void OnItemPreChange(FAbilityItem& InNewItem) override;

	virtual void OnItemChanged(FAbilityItem& InOldItem) override;

public:
	EDWEquipPartType GetPartType() const { return PartType; }

	void SetPartType(EDWEquipPartType val) { PartType = val; }
};
