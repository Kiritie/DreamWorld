// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Task/Base/TaskBase.h"
#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetTaskPanel.generated.h"

class UWidgetTaskCategory;
class UWidgetTaskRootItem;
class UVerticalBox;
class UWidgetTaskInfoItem;
class UWidgetTaskItem;
class UScrollBox;
class UWrapBox;
class UCommonButton;

/**
 * 制作面板
 */
UCLASS()
class DREAMWORLD_API UWidgetTaskPanel : public UUserWidgetBase
{
	GENERATED_BODY()

	friend class UWidgetTaskRootItem;
	friend class UWidgetTaskItem;
	
public:
	UWidgetTaskPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnReset(bool bForce) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery = false) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnTaskRootItemSelected(UWidgetTaskRootItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnTaskRootItemDeselected(UWidgetTaskRootItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnTaskItemSelected(UWidgetTaskItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnTaskItemDeselected(UWidgetTaskItem* InItem);

	UFUNCTION()
	void OnTaskContentRefresh();

	UFUNCTION()
	void OnTaskInfoContentRefresh();

	UFUNCTION()
	void OnStartTaskButtonClicked();

	UFUNCTION()
	void OnCreateTaskInfoItem(UTaskBase* InTask);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UScrollBox* TaskContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UVerticalBox* TaskInfoContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnStartTask;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskCategory> TaskCategoryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskRootItem> TaskRootItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskItem> TaskItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskInfoItem> TaskInfoItemClass;

protected:
	UPROPERTY()
	TArray<UWidgetTaskCategory*> TaskCategories;
	
	UPROPERTY()
	TArray<UWidgetTaskRootItem*> TaskRootItems;
	
	UPROPERTY()
	TArray<UWidgetTaskInfoItem*> TaskInfoItems;

	UPROPERTY()
	UWidgetTaskRootItem* SelectedTaskRootItem;

	UPROPERTY()
	UWidgetTaskItem* SelectedTaskItem;
};
