// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetTaskInfoItem.generated.h"

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskInfoItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidgetTaskInfoItem(const FObjectInitializer& ObjectInitializer);
};
