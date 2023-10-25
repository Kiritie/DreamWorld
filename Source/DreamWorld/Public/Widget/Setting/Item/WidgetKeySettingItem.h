// Fill out your copyright notice in the Description Item of Project Settings.

#pragma once
#include "WidgetSettingItemBase.h"

#include "WidgetKeySettingItem.generated.h"

class UWidgetButtonBase;
class UEditableTextBox;
class UWidgetPressAnyKeyPanel;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetKeySettingItem : public UWidgetSettingItemBase
{
	GENERATED_BODY()
	
public:
	UWidgetKeySettingItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy() override;

protected:
	void OnValueButtonClicked();

	void OnKeySelected(FKey InKey, UWidgetPressAnyKeyPanel* InPressAnyKeyPanel);

	void OnKeySelectionCanceled(UWidgetPressAnyKeyPanel* InPressAnyKeyPanel);

public:
	virtual FParameter GetValue() const override;

	virtual void SetValue(const FParameter& InValue) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UWidgetButtonBase* Btn_Value;
};
