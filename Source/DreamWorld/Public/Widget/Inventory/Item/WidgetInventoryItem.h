// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Widget/WidgetAbilityInventoryItemBase.h"
#include "WidgetInventoryItem.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryItem : public UWidgetAbilityInventoryItemBase
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
