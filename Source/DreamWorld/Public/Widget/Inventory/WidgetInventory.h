// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Inventory/Widget/WidgetInventoryBase.h"

#include "WidgetInventory.generated.h"

/**
 * ��Ʒ������
 */
UCLASS()
class DREAMWORLD_API UWidgetInventory : public UWidgetInventoryBase
{
	GENERATED_BODY()

public:
	UWidgetInventory(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;
	
	virtual void OnRefresh_Implementation() override;
};
