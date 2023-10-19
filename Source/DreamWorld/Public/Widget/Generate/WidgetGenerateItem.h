// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Widget/Item/WidgetAbilityItem.h"

#include "WidgetGenerateItem.generated.h"

/**
 * UI构建栏
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetGenerateItem : public UWidgetAbilityItem
{
	GENERATED_BODY()
	
public:
	UWidgetGenerateItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize_Implementation(const TArray<FParameter>& InParams) override;

public:
	virtual void OnSelected_Implementation() override;

	virtual void OnUnSelected_Implementation() override;

public:
	UPROPERTY(BlueprintReadOnly)
	FDWGenerateItemData GenerateItemData;
};
