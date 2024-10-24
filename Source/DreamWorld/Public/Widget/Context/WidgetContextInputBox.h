// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetContextInputBox.generated.h"

class UCommonButton;
class UEditableTextBox;
/**
 * 上下文框
 */
UCLASS()
class DREAMWORLD_API UWidgetContextInputBox : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetContextInputBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

protected:
	UFUNCTION()
	virtual void OnTextBoxValueCommitted(const FText& InText, ETextCommit::Type InCommitMethod);
		
	UFUNCTION()
	virtual void OnSendButtonClicked();

	UFUNCTION()
	virtual void OnUIMaskPressed();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UEditableTextBox* TxtBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnSend;
};
