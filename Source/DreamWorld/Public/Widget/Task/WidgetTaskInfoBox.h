// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetTaskInfoBox.generated.h"

class UVerticalBox;
class UWidgetTaskInfoItem;
class UTaskBase;

/**
 * 任务信息框
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskInfoBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetTaskInfoBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

protected:
	UFUNCTION()
	void OnTaskInfoContentRefresh();

	UFUNCTION()
	void OnCreateTaskInfoItem(UTaskBase* InTask);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UVerticalBox* ContentBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskInfoItem> TaskInfoItemClass;

protected:
	UPROPERTY()
	TArray<UWidgetTaskInfoItem*> TaskInfoItems;
};
