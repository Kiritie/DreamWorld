// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetArchiveCreatingPanel.generated.h"

class UWidgetArchiveCreatingPage;
class UCommonAnimatedSwitcher;
/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetArchiveCreatingPanel : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonAnimatedSwitcher* Switcher_Page;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPageIndex;

public:
	UFUNCTION(BlueprintPure)
	int32 GetCurrentPageIndex() const { return CurrentPageIndex; }

	UFUNCTION(BlueprintPure)
	UWidgetArchiveCreatingPage* GetCurrentPage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCurrentPage(int32 InPageIndex);
};
