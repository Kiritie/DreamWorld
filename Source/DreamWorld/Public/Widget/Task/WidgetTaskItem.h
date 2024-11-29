// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Widget/Sub/SubButtonWidgetBase.h"

#include "WidgetTaskItem.generated.h"

class UWidgetTaskContainer;
class UTaskBase;
class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskItem : public USubButtonWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTaskItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

public:
	virtual void OnAddToContainer(UWidgetTaskContainer* InTaskContainer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtLevel;

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
