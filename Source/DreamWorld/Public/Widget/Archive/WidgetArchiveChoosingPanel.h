// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetArchiveChoosingPanel.generated.h"

/**
 * ��ɫѡ�����
 */
UCLASS()
class DREAMWORLD_API UWidgetArchiveChoosingPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetArchiveChoosingPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;
};
