// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "WidgetInventoryPreviewSlot.h"
#include "WidgetInventoryShopSlot.generated.h"

/**
 * UI构建槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryShopSlot : public UWidgetInventoryPreviewSlot
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryShopSlot(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(UUserWidgetBase* InOwner, FAbilityItem InItem) override;
};
