// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetInventorySlot.h"
#include "WidgetInventoryAuxiliarySlot.generated.h"

/**
 * UI技能槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryAuxiliarySlot : public UWidgetInventorySlot
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryAuxiliarySlot(const FObjectInitializer& ObjectInitializer);
};
