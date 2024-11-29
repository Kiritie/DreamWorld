// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Setting/Widget/Page/WidgetSettingPageBase.h"

#include "WidgetGameSettingPage.generated.h"

class UWidgetBoolSettingItemBase;
class UWidgetEnumSettingItemBase;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetGameSettingPage : public UWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetGameSettingPage(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnReset(bool bForce) override;

	virtual void OnApply() override;

	virtual void OnActivated() override;

	virtual void OnDeactivated() override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;

protected:
	virtual FSaveData* GetDefaultSaveData() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItemBase* SettingItem_GameLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetBoolSettingItemBase* SettingItem_AutoJump;
};
