// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetMainMenu.generated.h"

/**
 * 主菜单
 */
UCLASS()
class DREAMWORLD_API UWidgetMainMenu : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetMainMenu(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;
};
