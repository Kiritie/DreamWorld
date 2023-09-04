// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetInventoryItem.h"

#include "WidgetInventoryShopItem.generated.h"

/**
 * UI商店栏
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryShopItem : public UWidgetInventoryItem
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryShopItem(const FObjectInitializer& ObjectInitializer);
};
