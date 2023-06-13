// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetItemCategoryBar.generated.h"

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetItemCategoryBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidgetItemCategoryBar(const FObjectInitializer& ObjectInitializer);
};
