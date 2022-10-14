// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetInventoryItemInfoBox.generated.h"

/**
 * ��ɫ���
 */
UCLASS()
class DREAMWORLD_API UWidgetInventoryItemInfoBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetInventoryItemInfoBox(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TextBlock_ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TextBlock_ItemDetail;

public:
	virtual void OnCreate_Implementation(AActor* InOwner) override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
