// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetTaskInfoBox.generated.h"

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
	virtual void OnCreate_Implementation(UObject* InOwner) override;

	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

	virtual void OnRefresh_Implementation() override;

protected:
	void CreateTaskInfoItem(UTaskBase* InTask);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UVerticalBox* ContentBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UWidgetTaskInfoItem> TaskInfoItemClass;
};
