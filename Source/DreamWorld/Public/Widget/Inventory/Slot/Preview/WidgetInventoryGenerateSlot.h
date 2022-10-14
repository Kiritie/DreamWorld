// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "WidgetInventoryPreviewSlot.h"
#include "WidgetInventoryGenerateSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGenerateSlotSelected, UWidgetInventoryGenerateSlot*, InSlot);

/**
 * UI构建槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryGenerateSlot : public UWidgetInventoryPreviewSlot
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryGenerateSlot(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGenerateSlotSelected OnSlotSelected;

public:
	virtual void OnDespawn_Implementation() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual void OnInitialize_Implementation(UUserWidgetBase* InOwner, FAbilityItem InItem) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSelectState(bool bSelected);
};
