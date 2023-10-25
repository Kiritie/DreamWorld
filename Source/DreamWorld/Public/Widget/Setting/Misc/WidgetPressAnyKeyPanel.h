// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"

#include "WidgetPressAnyKeyPanel.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetPressAnyKeyPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetPressAnyKeyPanel(const FObjectInitializer& Initializer);
	
public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

protected:
	void HandleKeySelected(FKey InKey);
	
	void HandleKeySelectionCanceled();

	void Dismiss(TFunction<void()> PostDismissCallback);

public:
	DECLARE_EVENT_OneParam(UWidgetPressAnyKeyPanel, FOnKeySelected, FKey);
	FOnKeySelected OnKeySelected;
	
	DECLARE_EVENT(UWidgetPressAnyKeyPanel, FOnKeySelectionCanceled);
	FOnKeySelectionCanceled OnKeySelectionCanceled;

private:
	bool bKeySelected = false;
	TSharedPtr<class FPressAnyKeyInputProcessor> InputProcessor;
};
