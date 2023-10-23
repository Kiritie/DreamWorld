// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Inventory/WidgetInventory.h"
#include "WidgetInventoryBox.generated.h"

class UWidgetInventorySlot;

/**
 * ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UWidgetInventoryBox : public UWidgetInventory
{
	GENERATED_BODY()

public:
	UWidgetInventoryBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UWrapBox* DefaultContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UWrapBox* TargetContent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<ESlotSplitType, FWidgetInventorySlots> TargetUISlotDatas;
};
