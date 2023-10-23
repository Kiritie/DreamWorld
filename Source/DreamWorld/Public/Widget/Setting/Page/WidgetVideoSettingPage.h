// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetSettingPageBase.h"
#include "Common/DWCommonTypes.h"

#include "WidgetVideoSettingPage.generated.h"

class UWidgetEnumSettingItem;

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
	UWidgetEnumSettingItem* SettingItem_GlobalVideoQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_ViewDistanceQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_ShadowQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_GlobalIlluminationQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_ReflectionQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_AntiAliasingQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_TextureQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_VisualEffectQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_PostProcessingQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_FoliageQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetEnumSettingItem* SettingItem_ShadingQuality;

public:
	UFUNCTION(BlueprintPure)
	FDWVideoModuleSaveData& GetDefaultVideoData() const;
};
