// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetShopPanel.generated.h"

/**
 * 商店面板
 */
UCLASS()
class DREAMWORLD_API UWidgetShopPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetShopPanel(const FObjectInitializer& ObjectInitializer);
};
