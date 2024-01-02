// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetInteractionBox.generated.h"

/**
 * 交互框
 */
UCLASS()
class DREAMWORLD_API UWidgetInteractionBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnClose(bool bInstant) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteractActions(const TArray<EInteractAction>& InActions);
		
	UFUNCTION(BlueprintImplementableEvent)
	void HideInteractActions();
};
