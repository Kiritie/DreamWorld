// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Inventory/CharacterInventory.h"
#include "DWCharacterInventory.generated.h"

class UInventoryEquipSlot;

/**
 * ��ɫ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UDWCharacterInventory : public UCharacterInventory
{
	GENERATED_BODY()

public:
	UDWCharacterInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(EPhase InPhase) override;

public:
	virtual FQueryItemInfo QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1) override;

	virtual void AddItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot *>& InSlots) override;

	virtual void RemoveItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot *>& InSlots) override;

	virtual void DiscardAllItem() override;

	virtual void ClearAllItem() override;

	virtual UInventoryEquipSlot* GetEquipSlotByPartType(EDWEquipPartType InPartType);
};
