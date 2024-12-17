// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Sub/SubWidgetBase.h"

#include "WidgetTaskCategory.generated.h"

class UWidgetTaskContainer;
class UVerticalBox;
class UTaskAsset;
class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskCategory : public USubWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTaskCategory(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetTaskContainer* TaskContainer;

protected:
	UPROPERTY(BlueprintReadOnly)
	FText TaskName;

public:
	FText GetTaskName() const { return TaskName; }
	
	UWidgetTaskContainer* GetTaskContainer() const { return TaskContainer; }
};
