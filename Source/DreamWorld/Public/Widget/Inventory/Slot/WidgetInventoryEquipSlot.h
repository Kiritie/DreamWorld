// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "WidgetInventoryEquipSlot.generated.h"

/**
 * UIװ����
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryEquipSlot : public UWidgetInventorySlot
{
	GENERATED_BODY()

public:
	UWidgetInventoryEquipSlot(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EquipPart;

public:
	FText GetEquipPart() const { return EquipPart; }

	void SetEquipPart(const FText& InEquipPart) { this->EquipPart = InEquipPart; }
};
