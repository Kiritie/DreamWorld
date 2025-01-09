// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetGeneratePanel.generated.h"

class UWidgetAbilityItemCategoryBar;
class UWidgetAbilityItem;
class UWidgetGenerateItem;
class UScrollBox;
class UWrapBox;
class UCommonButton;

/**
 * 制作面板
 */
UCLASS()
class DREAMWORLD_API UWidgetGeneratePanel : public UUserWidgetBase
{
	GENERATED_BODY()

	friend class UWidgetGenerateItem;

public:
	UWidgetGeneratePanel(const FObjectInitializer& ObjectInitializer);

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
	void OnItemCategorySelected(EAbilityItemType InItemType);

	UFUNCTION(BlueprintNativeEvent)
	void OnGenerateItemSelected(UWidgetGenerateItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnGenerateItemDeselected(UWidgetGenerateItem* InItem);

	UFUNCTION()
	void OnGenerateContentRefresh(bool bScrollToStart = false);

	UFUNCTION()
	void OnPreviewContentRefresh();

	UFUNCTION()
	void OnGenerateButtonClicked();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetAbilityItemCategoryBar* CategoryBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UScrollBox* GenerateContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWrapBox* PreviewContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnGenerate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetGenerateItem> GenerateItemClass;

protected:
	UPROPERTY()
	TArray<UWidgetGenerateItem*> GenerateItems;
	
	UPROPERTY()
	TArray<UWidgetAbilityItem*> PreviewItems;

	UPROPERTY()
	UWidgetGenerateItem* SelectedGenerateItem;

	UPROPERTY()
	TArray<FAbilityItem> SelectedPreviewItems;

	UPROPERTY()
	FPrimaryAssetId GenerateToolID;
	
	UPROPERTY()
	int32 GenerateRawDataIndex;
	
	UPROPERTY()
	FTimerHandle ContentRefreshTH;

public:
	UFUNCTION(BlueprintPure)
	bool GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData) const;
};
