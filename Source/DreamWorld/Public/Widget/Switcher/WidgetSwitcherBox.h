// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Sub/SubWidgetBase.h"

#include "WidgetSwitcherBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitcherButtonSelected, int32, ButtonIndex);

class UCommonButtonBase;
class UCommonButtonGroup;
/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetSwitcherBox : public USubWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetSwitcherBox(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

protected:
	UFUNCTION()
	void OnSwitcherButtonSelected(UCommonButtonBase* AssociatedButton, int32 ButtonIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UPanelWidget* SwitcherContent;

	UPROPERTY()
	UCommonButtonGroup* SwitcherGroup;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSwitcherButtonSelected OnButtonSelected;

public:
	UFUNCTION(BlueprintPure)
	int32 GetSelectedButtonIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetSelectedButtonIndex(int32 InButtonIndex) const;
};
