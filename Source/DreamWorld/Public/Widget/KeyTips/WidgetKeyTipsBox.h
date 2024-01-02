// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetKeyTipsBox.generated.h"

/**
 * 上下文框
 */
UCLASS()
class DREAMWORLD_API UWidgetKeyTipsBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetKeyTipsBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;
};
