// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Common/DWCommonTypes.h"
#include "DWCharacterInventory.generated.h"

class UAbilityInventoryEquipSlot;

/**
 * ��ɫ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UDWCharacterInventory : public UAbilityCharacterInventoryBase
{
	GENERATED_BODY()

public:
	UDWCharacterInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

	virtual void UnloadData(EPhase InPhase) override;

public:
	virtual FQueryItemInfo QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1) override;

	virtual void AddItemBySlots(FAbilityItem& InItem, const TArray<UAbilityInventorySlot *>& InSlots) override;

	virtual void RemoveItemBySlots(FAbilityItem& InItem, const TArray<UAbilityInventorySlot *>& InSlots) override;

	virtual void DiscardAllItem() override;

	virtual void ClearAllItem() override;

	virtual UAbilityInventoryEquipSlot* GetEquipSlotByPartType(EDWEquipPartType InPartType);
};
