// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Sub/SubWidgetBase.h"
#include "WidgetTaskInfo.generated.h"

class UTextBlock;
class UTaskBase;

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskInfo : public USubWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetTaskInfo(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;
	
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtDetail;

protected:
	UPROPERTY(BlueprintReadOnly)
	UTaskBase* Task;

public:
	UTaskBase* GetTask() const { return Task; }
};
