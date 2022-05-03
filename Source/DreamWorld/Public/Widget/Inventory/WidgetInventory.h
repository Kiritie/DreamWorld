// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"

#include "WidgetInventory.generated.h"

class UWidgetInventorySlot;
class UWidgetInventoryEquipSlot;

/**
 * ��Ʒ������
 */
UCLASS()
class DREAMWORLD_API UWidgetInventory : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetInventory(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<ESplitSlotType, FWidgetSplitSlotData> UISlotDatas;

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;
	
	virtual void OnRefresh_Implementation() override;

	UFUNCTION(BlueprintPure)
	UInventory* GetInventory() const;

	UFUNCTION(BlueprintPure)
	TArray<UWidgetInventorySlot*> GetSplitUISlots(ESplitSlotType InSplitSlotType) const;
};