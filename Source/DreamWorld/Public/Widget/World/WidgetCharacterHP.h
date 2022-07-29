// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/World/WorldWidgetBase.h"

#include "WidgetCharacterHP.generated.h"

/**
 * ��ɫѪ��
 */
UCLASS()
class DREAMWORLD_API UWidgetCharacterHP : public UWorldWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetCharacterHP(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHeadInfo(const FString& InHeadInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealthPercent(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetManaPercent(float InMana, float InMaxMana);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStaminaPercent(float InStamina, float InMaxStamina);
};
