// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Screen/UMG/UserWidgetBase.h"

#include "WidgetSettingPageBase.generated.h"

class UScrollBox;
class UWidgetSettingItemBase;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetSettingPageBase : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetSettingPageBase(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnCreate(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnReset() override;

	virtual void OnClose(bool bInstant) override;

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnApply")
	void K2_OnApply();
	UFUNCTION()
	virtual void OnApply();

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnValueChange")
	void K2_OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue);
	UFUNCTION()
	virtual void OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanApply() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanReset() const;

public:
	UFUNCTION(BlueprintCallable)
	void Apply();

public:
	UFUNCTION(BlueprintNativeEvent)
	void AddSettingItem(UWidgetSettingItemBase* InSettingItem, const FText& InCategory = FText::GetEmpty());

	UFUNCTION(BlueprintNativeEvent)
	void ClearSettingItems();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UScrollBox* ContentBox;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Title;

private:
	FText LastCategory;
};
