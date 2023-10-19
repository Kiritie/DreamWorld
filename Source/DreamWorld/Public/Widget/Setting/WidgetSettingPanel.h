// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Screen/UMG/UserWidgetBase.h"

#include "WidgetSettingPanel.generated.h"

class UCommonButtonBase;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetSettingPanel : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetSettingPanel(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

public:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void ChangePage(int32 InPageIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UTextBlock* Txt_Title;

public:
	UPROPERTY(BlueprintReadOnly)
	int32 PageIndex;
};
