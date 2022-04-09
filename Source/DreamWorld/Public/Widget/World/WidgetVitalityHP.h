// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/World/WorldWidgetBase.h"

#include "WidgetVitalityHP.generated.h"

class ADWVitality;

/**
 * ������Ѫ��
 */
UCLASS()
class DREAMWORLD_API UWidgetVitalityHP : public UWorldWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetVitalityHP(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	ADWVitality* OwnerObject;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHeadInfo(const FString& InHeadInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealthPercent(float InHealth, float InMaxHealth);

	ADWVitality* GetOwnerObject() const { return OwnerObject; }

	void SetOwnerObject(ADWVitality* val) { OwnerObject = val; }
};
