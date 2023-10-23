// Fill out your copyright notice in the Description Item of Project Settings.

#pragma once
#include "ObjectPool/ObjectPoolInterface.h"
#include "Widget/Screen/UMG/SubWidgetBase.h"

#include "WidgetSettingItemBase.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingItemValueChanged, UWidgetSettingItemBase*, InSettingItem, const FParameter&, InValue);

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetSettingItemBase : public USubWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetSettingItemBase(const FObjectInitializer& ObjectInitializer);

public:
	virtual int32 GetLimit_Implementation() const override { return 1000; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy() override;

public:
	UFUNCTION(BlueprintPure)
	FText GetLabel() const;

	UFUNCTION(BlueprintCallable)
	void SetLabel(FText InText);

public:
	UFUNCTION(BlueprintPure)
	virtual FParameter GetValue() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetValue(const FParameter& InValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UTextBlock* Txt_Label;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSettingItemValueChanged OnValueChanged;
};
