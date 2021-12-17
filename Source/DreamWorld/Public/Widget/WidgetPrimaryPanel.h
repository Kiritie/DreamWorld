// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/User/UserWidgetBase.h"
#include "WidgetPrimaryPanel.generated.h"

class ADWPlayerCharacter;

/**
 * 主界面
 */
UCLASS()
class DREAMWORLD_API UWidgetPrimaryPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetPrimaryPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHeadInfo(const FString& InHeadInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealthPercent(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetManaPercent(float InMana, float InMaxMana);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStaminaPercent(float InStamina, float InMaxStamina);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetCrosshairVisible(bool bValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowMessage(const FString& InMessage, float InDuration = 2.5f);
				
	UFUNCTION(BlueprintCallable)
	void RefreshActions();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowActions(const TArray<EInteractAction>& InActions);
		
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HideActions();

	UFUNCTION()
	void OnChangeInputMode(EInputMode InInputMode);
};
