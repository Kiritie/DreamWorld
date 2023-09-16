// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Global/DWGlobalTypes.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetGeneratePanel.generated.h"

class UScrollBox;
class UWrapBox;
class UWidgetInventoryGenerateItem;
class UWidgetInventoryPreviewItem;
class UButton;

/**
 * 制作面板
 */
UCLASS()
class DREAMWORLD_API UWidgetGeneratePanel : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetGeneratePanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate_Implementation(UObject* InOwner) override;

	virtual void OnInitialize_Implementation(UObject* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

	virtual void OnRefresh_Implementation() override;

	virtual void OnDestroy_Implementation(bool bRecovery = false) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnGenerateSlotSelected(UWidgetInventoryGenerateItem* InSlot);

	UFUNCTION()
	void OnPreviewContentRefresh();

	UFUNCTION()
	void OnGenerateButtonClicked();

public:
	UFUNCTION(BlueprintPure)
	bool GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UScrollBox* GenerateContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UWrapBox* PreviewContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UButton* BtnGenerate;

protected:
	TSubclassOf<UWidgetInventoryGenerateItem> GenerateSlotClass;

	TSubclassOf<UWidgetInventoryPreviewItem> PreviewSlotClass;

	TArray<UWidgetInventoryGenerateItem*> GenerateItems;
	
	TArray<UWidgetInventoryPreviewItem*> PreviewItems;

	UPROPERTY()
	UWidgetInventoryGenerateItem* SelectedGenerateSlot;

	UPROPERTY()
	FDWGenerateRawData SelectedGenerateRawData;

	UPROPERTY()
	int32 PreviewGenerateRawDataIndex;

	UPROPERTY()
	FPrimaryAssetId GenerateToolID;
	
	UPROPERTY()
	FTimerHandle PreviewContentRefreshTH;

	UPROPERTY(BlueprintReadOnly)
	UObject* TargetObject;
};
