// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"

#include "WidgetPausingMenu.generated.h"

/**
 * 暂停菜单
 */
UCLASS()
class DREAMWORLD_API UWidgetPausingMenu : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetPausingMenu(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
