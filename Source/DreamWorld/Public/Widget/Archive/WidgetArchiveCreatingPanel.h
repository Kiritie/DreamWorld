// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/User/UserWidgetBase.h"
#include "WidgetArchiveCreatingPanel.generated.h"

/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetArchiveCreatingPanel : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnRefresh_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ResetData();
};
