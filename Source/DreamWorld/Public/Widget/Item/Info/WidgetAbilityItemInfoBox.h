// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetAbilityItemInfoBox.generated.h"

/**
 * ��ɫ���
 */
UCLASS()
class DREAMWORLD_API UWidgetAbilityItemInfoBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetAbilityItemInfoBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;
};
