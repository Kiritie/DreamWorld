// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"

#include "WidgetPausingMenu.generated.h"

/**
 * 暂停菜单
 */
UCLASS()
class DREAMWORLD_API UWidgetPausingMenu : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetPausingMenu(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
