// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetMessageBox.generated.h"

/**
 * 物品信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetMessageBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetMessageBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation(UObject* InOwner) override;

	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
