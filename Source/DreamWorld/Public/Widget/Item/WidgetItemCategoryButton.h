// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetItemCategoryButton.generated.h"

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetItemCategoryButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidgetItemCategoryButton(const FObjectInitializer& ObjectInitializer);
};
