// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Widget/WidgetAbilityItemBase.h"
#include "WidgetAbilityItem.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetAbilityItem : public UWidgetAbilityItemBase
{
	GENERATED_BODY()
	
public:
	UWidgetAbilityItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

public:
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnSelected")
	void K2_OnSelected();
	virtual void OnSelected();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnUnSelected")
	void K2_OnUnSelected();
	virtual void OnUnSelected();
};
