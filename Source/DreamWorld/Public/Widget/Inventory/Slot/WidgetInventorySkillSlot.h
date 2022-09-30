// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInventorySlot.h"
#include "WidgetInventorySkillSlot.generated.h"

class UWidgetInventory;

/**
 * UI技能槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventorySkillSlot : public UWidgetInventorySlot
{
	GENERATED_BODY()
	
public:
	UWidgetInventorySkillSlot(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UInventorySlot* InOwnerSlot) override;

protected:
	virtual void OnRefresh() override;

public:
	virtual void UseItem(int InCount) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtName;
};
