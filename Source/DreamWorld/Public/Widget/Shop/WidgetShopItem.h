// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Item/WidgetAbilityItem.h"

#include "WidgetShopItem.generated.h"

/**
 * UI商店栏
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetShopItem : public UWidgetAbilityItem
{
	GENERATED_BODY()
	
public:
	UWidgetShopItem(const FObjectInitializer& ObjectInitializer);
};
