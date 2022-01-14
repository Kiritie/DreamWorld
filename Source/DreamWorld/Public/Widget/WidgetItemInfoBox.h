// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/User/UserWidgetBase.h"
#include "WidgetItemInfoBox.generated.h"

class ADWPlayerCharacter;

/**
 * 主界面
 */
UCLASS()
class DREAMWORLD_API UWidgetItemInfoBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetItemInfoBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation() override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
