// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Pool/PoolWidgetBase.h"

#include "WidgetTaskContainer.generated.h"

class UWidgetTaskItem;
class UVerticalBox;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskContainer : public UPoolWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTaskContainer(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnRefresh() override;

public:
	UFUNCTION(Blueprintable)
	void AddTaskItem(UWidgetTaskItem* InTaskItem);

	UFUNCTION(Blueprintable)
	void ClearTaskItem();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UVerticalBox* TaskContent;

protected:
	UPROPERTY()
	TArray<UWidgetTaskItem*> TaskItems;

public:
	TArray<UWidgetTaskItem*> GetTaskItems() const { return TaskItems; }
};
