// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Setting/Widget/Page/Module/WidgetVideoSettingPageBase.h"

#include "WidgetVideoSettingPage.generated.h"

class UWidgetFloatSettingItemBase;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetVideoSettingPage : public UWidgetVideoSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnReset(bool bForce) override;

	virtual void OnApply() override;

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

	virtual void OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue) override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;

protected:
	virtual FSaveData* GetDefaultSaveData() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItemBase* SettingItem_VoxelWorldRange;
};
