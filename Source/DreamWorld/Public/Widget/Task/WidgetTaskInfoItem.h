// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/SubWidgetBase.h"
#include "WidgetTaskInfoItem.generated.h"

class UTaskBase;

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskInfoItem : public USubWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetTaskInfoItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh_Implementation() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	UTaskBase* Task;
};
