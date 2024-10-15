// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Item/WidgetAbilityItem.h"

#include "WidgetTransactionItem.generated.h"

/**
 * UI交易项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTransactionItem : public UWidgetAbilityItem
{
	GENERATED_BODY()
	
public:
	UWidgetTransactionItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;
};
