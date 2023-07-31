// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/VitalityInventory.h"
#include "DWVitalityInventory.generated.h"

/**
 * �����Ʒ��
 */
UCLASS()
class DREAMWORLD_API UDWVitalityInventory : public UVitalityInventory
{
	GENERATED_BODY()

public:
	UDWVitalityInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

	virtual void UnloadData(EPhase InPhase) override;

public:
	virtual void DiscardAllItem() override;

	virtual void ClearAllItem() override;

	virtual FQueryItemInfo QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1) override;
};
