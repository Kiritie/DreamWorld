// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/AbilityModuleTypes.h"
#include "Task/Base/TaskBase.h"
#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetTaskPanel.generated.h"

class UWidgetAbilityItem;
class UWidgetTaskContainer;
class UWidgetTaskCategory;
class UWidgetTaskRootItem;
class UVerticalBox;
class UWidgetTaskInfo;
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

	friend class UWidgetTaskCategory;
	friend class UWidgetTaskItem;
	friend class UWidgetTaskRootItem;
	
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
	void OnTaskContentRefresh(bool bScrollToStart = false);

	UFUNCTION()
	void OnTaskInfoContentRefresh();

	UFUNCTION()
	void OnPreviewContentRefresh();

	UFUNCTION()
	void OnTraceTaskButtonClicked();

	UFUNCTION()
	void OnCreateTaskInfo(UTaskBase* InTask);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UScrollBox* TaskContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UVerticalBox* TaskInfoContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWrapBox* PreviewContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnTraceTask;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskCategory> TaskCategoryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskRootItem> TaskRootItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskItem> TaskItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskInfo> TaskInfoClass;

protected:
	UPROPERTY()
	TArray<UWidgetTaskCategory*> TaskCategories;
	
	UPROPERTY()
	TArray<UWidgetTaskInfo*> TaskInfos;
	
	UPROPERTY()
	TArray<UWidgetAbilityItem*> PreviewItems;

	UPROPERTY()
	TArray<FAbilityItem> SelectedPreviewItems;

	UPROPERTY()
	UWidgetTaskRootItem* SelectedTaskRootItem;

	UPROPERTY()
	UWidgetTaskItem* SelectedTaskItem;

public:
	UFUNCTION(BlueprintPure)
	bool GetSelectedTask(UTaskBase*& OutTask) const;

	UFUNCTION(BlueprintCallable)
	UWidgetTaskCategory* GetOrCreateTaskCategory(UTaskAsset* InTaskAsset);
};
