// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetInventoryItem.h"
#include "WidgetInventoryPreviewItem.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryPreviewItem : public UWidgetInventoryItem
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryPreviewItem(const FObjectInitializer& ObjectInitializer);
};
