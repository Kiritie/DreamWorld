// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ability/AbilityModuleTypes.h"
#include "Widget/Screen/SubWidgetBase.h"

#include "WidgetAbilityItemInfo.generated.h"

class UTextBlock;

/**
 * ��ɫ���
 */
UCLASS()
class DREAMWORLD_API UWidgetAbilityItemInfo : public USubWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetAbilityItemInfo(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtItemLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtItemDetail;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FAbilityItem Item;

public:
	UFUNCTION(BlueprintPure)
	FAbilityItem GetItem() const { return Item; }
};
