// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetInventorySlot.h"
#include "WidgetInventorySkillSlot.generated.h"

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
	virtual void OnInitialize_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh_Implementation() override;

public:
	virtual void UseItem_Implementation(int InCount) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtName;
};
