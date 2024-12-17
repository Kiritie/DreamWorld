// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetTaskItem.h"
#include "Common/DWCommonTypes.h"

#include "WidgetTaskRootItem.generated.h"

class UWidgetTaskContainer;
class UVerticalBox;
class UWidgetTaskCategory;
class UTaskBase;
class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskRootItem : public UWidgetTaskItem
{
	GENERATED_BODY()
	
public:
	UWidgetTaskRootItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery) override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

public:
	virtual void OnAddToContainer(UWidgetTaskContainer* InTaskContainer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTaskContainer> TaskContainerClass;

	UPROPERTY(BlueprintReadOnly)
	UTaskAsset* TaskAsset;

	UPROPERTY(BlueprintReadOnly)
	UWidgetTaskContainer* TaskContainer;

public:
	UTaskAsset* GetTaskAsset() const { return TaskAsset; }
	
	UWidgetTaskContainer* GetTaskContainer() const { return TaskContainer; }
};
