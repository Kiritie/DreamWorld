// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "InventorySlot.h"
#include "InventoryShortcutSlot.generated.h"

class UCharacterInventory;

/**
 * ���ܲ�
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UInventoryShortcutSlot : public UInventorySlot
{
	GENERATED_BODY()

public:
	UInventoryShortcutSlot();

public:
	virtual void InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/) override;

	virtual void Refresh() override;

public:
	virtual bool IsSelected();
};
