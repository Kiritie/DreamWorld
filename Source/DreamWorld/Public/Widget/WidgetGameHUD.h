// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetGameHUD.generated.h"

class ADWPlayerCharacter;

/**
 * 游戏主界面
 */
UCLASS()
class DREAMWORLD_API UWidgetGameHUD : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetGameHUD(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnClose(bool bInstant) override;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetCrosshairVisible(bool bValue);

protected:
	UFUNCTION()
	void OnInputModeChanged(UObject* InSender, class UEventHandle_InputModeChanged* InEventHandle);
};
