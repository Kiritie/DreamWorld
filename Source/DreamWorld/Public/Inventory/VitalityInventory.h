// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Inventory/Inventory.h"
#include "VitalityInventory.generated.h"

class ADWVitality;
/**
 * �����Ʒ��
 */
UCLASS()
class DREAMWORLD_API UVitalityInventory : public UInventory
{
	GENERATED_BODY()

public:
	UVitalityInventory();

public:
	virtual void Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos) override;
	
	virtual void LoadData(FInventorySaveData InInventoryData, AActor* InOwner) override;

	virtual FInventorySaveData ToData() override;

	virtual void DiscardAllItem() override;

	virtual void ClearAllItem() override;

	virtual FQueryItemInfo GetItemInfoByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1) override;

public:
	UFUNCTION(BlueprintPure)
	ADWVitality* GetOwnerVitality() const;
};
