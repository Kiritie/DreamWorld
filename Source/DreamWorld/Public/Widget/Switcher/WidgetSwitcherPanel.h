// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetSwitcherPanel.generated.h"

class UWidgetSwitcherBox;
/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetSwitcherPanel : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetSwitcherPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

protected:
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION()
	void OnControlModeButtonSelected(int32 ButtonIndex);

	UFUNCTION()
	void OnWeaponGroupButtonSelected(int32 ButtonIndex);

	UFUNCTION()
	void OnGenerateToolButtonSelected(int32 ButtonIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetSwitcherBox* Switcher_ControlMode;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetSwitcherBox* Switcher_WeaponGroup;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetSwitcherBox* Switcher_GenerateTool;
};
