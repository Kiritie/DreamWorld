// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/AbilityModuleTypes.h"
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
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteractActions(const TArray<EInteractAction>& InActions);
		
	UFUNCTION(BlueprintImplementableEvent)
	void HideInteractActions();

protected:
	UFUNCTION()
	void OnChangeInputMode(UObject* InSender, class UEventHandle_ChangeInputMode* InEventHandle);
};
