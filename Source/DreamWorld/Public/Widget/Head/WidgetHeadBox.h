// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetHeadBox.generated.h"

class UWidgetTeamMateHeadBox;
class ADWCharacter;
class ADWPlayerCharacter;

/**
 * 头像框
 */
UCLASS()
class DREAMWORLD_API UWidgetHeadBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetHeadBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnClose(bool bInstant) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddTeamMate(ADWCharacter* InTeamMate);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemoveTeamMate(ADWCharacter* InTeamMate);
		
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClearTeamMates();
		
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	UWidgetTeamMateHeadBox* GetTeamMateHeadBoxWidget(ADWCharacter* InTeamMate) const;

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
	void SetHungerValue(float InHunger);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetThirstValue(float InThirst);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetOxygenValue(float InOxygen);
};
