// Fill out your copyright notice in the Description Item of Project Settings.

#pragma once
#include "WidgetSettingItemBase.h"

#include "WidgetFloatSettingItem.generated.h"

class USlider;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetFloatSettingItem : public UWidgetSettingItemBase
{
	GENERATED_BODY()
	
public:
	UWidgetFloatSettingItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual FParameter GetValue() const override;

	virtual void SetValue(const FParameter& InValue) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	USlider* Slider_Value;
};
