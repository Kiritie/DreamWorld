// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Widget/Item/WidgetAbilityItem.h"

#include "WidgetGenerateItem.generated.h"

class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetGenerateItem : public UWidgetAbilityItem
{
	GENERATED_BODY()
	
public:
	UWidgetGenerateItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtRarity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtLevel;

protected:
	UPROPERTY(BlueprintReadOnly)
	FDWGenerateItemData GenerateItemData;

public:
	FDWGenerateItemData GetGenerateItemData() const { return GenerateItemData; }
};
