// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Setting/Widget/WidgetSettingPanelBase.h"
#include "Widget/Screen/UserWidgetBase.h"

#include "WidgetSettingPanel.generated.h"

class UCommonButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetSettingPanel : public UWidgetSettingPanelBase
{
	GENERATED_BODY()
	
public:
	UWidgetSettingPanel(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
