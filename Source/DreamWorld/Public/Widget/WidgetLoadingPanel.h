// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/UMG/UserWidgetBase.h"

#include "WidgetLoadingPanel.generated.h"

/**
 * ���ؽ���
 */
UCLASS()
class DREAMWORLD_API UWidgetLoadingPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetLoadingPanel(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
