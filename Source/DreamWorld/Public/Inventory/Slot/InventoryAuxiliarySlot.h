// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "InventorySlot.h"
#include "InventoryAuxiliarySlot.generated.h"

class UCharacterInventory;

/**
 * ���ܲ�
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UInventoryAuxiliarySlot : public UInventorySlot
{
	GENERATED_BODY()

public:
	UInventoryAuxiliarySlot();

public:
	virtual void InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/) override;
};
