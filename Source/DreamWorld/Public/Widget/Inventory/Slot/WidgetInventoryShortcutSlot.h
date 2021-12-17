// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInventorySlot.h"
#include "WidgetInventoryShortcutSlot.generated.h"

class UWidgetInventory;

/**
 * UI技能槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryShortcutSlot : public UWidgetInventorySlot
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryShortcutSlot(const FObjectInitializer& ObjectInitializer);
};
