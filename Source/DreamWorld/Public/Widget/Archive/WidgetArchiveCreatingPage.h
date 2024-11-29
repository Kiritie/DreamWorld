// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Sub/SubActivatableWidgetBase.h"
#include "WidgetArchiveCreatingPage.generated.h"

/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetArchiveCreatingPage : public USubActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetArchiveCreatingPage(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

public:	
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	bool IsAllInputed() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Title;

public:
	FText GetTitle() const { return Title; }
};
