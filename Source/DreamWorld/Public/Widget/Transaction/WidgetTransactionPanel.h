// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonButtonBase.h"
#include "Common/DWCommonTypes.h"
#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetTransactionPanel.generated.h"

class UWidgetAbilityItem;
class UCommonButtonGroup;
class IAbilityInventoryAgentInterface;
class UScrollBox;
class UWrapBox;
class UWidgetTransactionItem;
class UWidgetAbilityItem;
class UCommonButton;

/**
 * 制作面板
 */
UCLASS()
class DREAMWORLD_API UWidgetTransactionPanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetTransactionPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnReset(bool bForce) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery = false) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnTabButtonSelected(UCommonButtonBase* SelectedTabButton, int32 ButtonIndex);

	UFUNCTION()
	void OnTransactionContentRefresh();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnTransactionItemSelected(UWidgetTransactionItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnTransactionItemDeselected(UWidgetTransactionItem* InItem);

	UFUNCTION()
	void OnPreviewContentRefresh();

	UFUNCTION()
	void OnTransactionButtonClicked();

public:
	UFUNCTION(BlueprintPure)
	int32 GetTabIndex() const;

	UFUNCTION(BlueprintPure)
	bool GetSelectedTransactionItem(FAbilityItem& OutItemData) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UPanelWidget* TabContent;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UScrollBox* TransactionContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWrapBox* PreviewContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnTransaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTransactionItem> TransactionItemClass;

protected:
	UPROPERTY()
	UCommonButtonGroup* TabGroup;

	UPROPERTY()
	TArray<UWidgetTransactionItem*> TransactionItems;
	
	UPROPERTY()
	TArray<UWidgetAbilityItem*> PreviewItems;

	UPROPERTY()
	UWidgetTransactionItem* SelectedTransactionItem;

	UPROPERTY()
	TArray<FAbilityItem> SelectedPreviewItems;

	IAbilityInventoryAgentInterface* TransactionTarget;
};
