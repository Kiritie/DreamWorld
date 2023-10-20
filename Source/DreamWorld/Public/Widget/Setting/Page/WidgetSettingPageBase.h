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
	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnCreate_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnReset_Implementation() override;

	virtual void OnClose_Implementation(bool bInstant) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnApply();

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
