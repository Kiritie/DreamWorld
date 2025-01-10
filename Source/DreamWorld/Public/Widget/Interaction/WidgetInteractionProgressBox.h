// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetInteractionProgressBox.generated.h"

class UTextBlock;
class IInteractionAgentInterface;
class UWidgetAbilityItem;
class UWidgetKeyTipsItemBase;

/**
 * 交互框
 */
UCLASS()
class DREAMWORLD_API UWidgetInteractionProgressBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetInteractionProgressBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnClose(bool bInstant) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtActionName;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetProgress(float InProgress);
};
