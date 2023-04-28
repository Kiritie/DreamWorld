// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetContextBox.generated.h"

/**
 * 上下文框
 */
UCLASS()
class DREAMWORLD_API UWidgetContextBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetContextBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation(AActor* InOwner) override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddMessage(const FString& InContent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearContext();
};
