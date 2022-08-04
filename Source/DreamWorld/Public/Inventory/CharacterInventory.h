// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Inventory/Inventory.h"
#include "CharacterInventory.generated.h"

class UInventoryEquipSlot;
class UInventorySkillSlot;

/**
 * ��ɫ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UCharacterInventory : public UInventory
{
	GENERATED_BODY()

public:
	UCharacterInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(bool bForceMode) override;

public:
	virtual FQueryItemInfo QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1) override;

	virtual void DiscardAllItem() override;

	virtual void ClearAllItem() override;
};
