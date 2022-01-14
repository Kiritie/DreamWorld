// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Inventory/WidgetInventory.h"
#include "WidgetInventoryBox.generated.h"

class UWidgetInventorySlot;

/**
 * ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UWidgetInventoryBox : public UWidgetInventory
{
	GENERATED_BODY()

public:
	UWidgetInventoryBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
