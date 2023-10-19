// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"

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
	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

public:
	virtual void SetLastTemporary(IScreenWidgetInterface* InLastTemporary) override { }
};
