// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Widget/Screen/SubWidgetBase.h"

#include "WidgetTaskRootItem.generated.h"

class UVerticalBox;
class UWidgetTaskCategory;
class UWidgetTaskItem;
class UTaskBase;
class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskRootItem : public USubWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTaskRootItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtDetail;

protected:
	UPROPERTY(BlueprintReadOnly)
	UTaskBase* Task;

	UPROPERTY(BlueprintReadOnly)
	UVerticalBox* TaskContent;

	UPROPERTY(BlueprintReadOnly)
	UWidgetTaskCategory* TaskCategory;
	
	UPROPERTY()
	TArray<UWidgetTaskItem*> TaskItems;

public:
	UTaskBase* GetTask() const { return Task; }
	
	UWidgetTaskCategory* GetTaskCategory() const { return TaskCategory; }
};
