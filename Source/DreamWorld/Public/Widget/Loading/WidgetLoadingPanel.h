// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"

#include "WidgetLoadingPanel.generated.h"

/**
 * 加载面板
 */
UCLASS()
class DREAMWORLD_API UWidgetLoadingPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetLoadingPanel(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual void SetLastTemporary(IScreenWidgetInterface* InLastTemporary) override { }
};
