// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetSettingPageBase.h"

#include "WidgetVideoSettingPage.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetVideoSettingPage : public UWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnCreate_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnApply_Implementation() override;

	virtual void OnReset_Implementation() override;

	virtual void OnClose_Implementation(bool bInstant) override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;
};
