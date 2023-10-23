// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Widget/WidgetAbilityInventoryBase.h"

#include "WidgetInventory.generated.h"

/**
 * ��Ʒ������
 */
UCLASS()
class DREAMWORLD_API UWidgetInventory : public UWidgetAbilityInventoryBase
{
	GENERATED_BODY()

public:
	UWidgetInventory(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner) override;
	
	virtual void OnRefresh() override;
};
