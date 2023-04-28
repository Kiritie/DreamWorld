// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetTaskInfoBox.generated.h"

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskInfoBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetTaskInfoBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation(AActor* InOwner) override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
