// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/UMG/UserWidgetBase.h"
#include "WidgetGameHUD.generated.h"

class ADWPlayerCharacter;

/**
 * 主界面
 */
UCLASS()
class DREAMWORLD_API UWidgetGameHUD : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetGameHUD(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation() override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

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
				
	UFUNCTION(BlueprintCallable)
	void RefreshActions();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowActions(const TArray<EInteractAction>& InActions);
		
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HideActions();

	UFUNCTION()
	void OnChangeInputMode(UObject* InSender, class UEventHandle_ChangeInputMode* InEventHandle);
};
