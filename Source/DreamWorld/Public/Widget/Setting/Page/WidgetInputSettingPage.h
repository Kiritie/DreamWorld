// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetSettingPageBase.h"
#include "Common/DWCommonTypes.h"

#include "WidgetInputSettingPage.generated.h"

class UWidgetKeySettingItem;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetInputSettingPage : public UWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetInputSettingPage(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnCreate(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnApply() override;

	virtual void OnReset() override;

	virtual void OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetKeySettingItem* SettingItem_Jump;

public:
	UFUNCTION(BlueprintPure)
	FDWInputModuleSaveData& GetDefaultInputData() const;
};
