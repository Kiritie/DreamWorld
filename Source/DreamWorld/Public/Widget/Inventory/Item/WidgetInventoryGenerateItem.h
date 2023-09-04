// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetInventoryItem.h"

#include "WidgetInventoryGenerateItem.generated.h"

/**
 * UI构建栏
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryGenerateItem : public UWidgetInventoryItem
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryGenerateItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnSelected_Implementation() override;

	virtual void OnUnSelected_Implementation() override;
};
