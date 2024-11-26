// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetInteractionBox.generated.h"

class IInteractionAgentInterface;
class UWidgetAbilityItem;
class UWidgetKeyTipsItemBase;

/**
 * 交互框
 */
UCLASS()
class DREAMWORLD_API UWidgetInteractionBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnClose(bool bInstant) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetAbilityItem* PreviewItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetKeyTipsItemBase* NextKeyTips;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IInteractionAgentInterface> InteractionAgent;

protected:
	UFUNCTION(BlueprintPure)
	TArray<EInteractAction> GetInteractActions() const;
	
	UFUNCTION()
	ESlateVisibility GetNextKeyTipsVisibility();
};
