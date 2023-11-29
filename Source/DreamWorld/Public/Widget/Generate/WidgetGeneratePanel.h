// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetGeneratePanel.generated.h"

class UScrollBox;
class UWrapBox;
class UWidgetGenerateItem;
class UWidgetAbilityPreviewItem;
class UCommonButton;

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
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery = false) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnGenerateSlotSelected(UWidgetGenerateItem* InSlot);

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
	UCommonButton* BtnGenerate;

protected:
	TArray<UWidgetGenerateItem*> GenerateItems;
	
	TArray<UWidgetAbilityPreviewItem*> PreviewItems;

	UPROPERTY()
	UWidgetGenerateItem* SelectedGenerateSlot;

	UPROPERTY()
	FDWGenerateRawData SelectedGenerateRawData;

	UPROPERTY()
	int32 PreviewGenerateRawDataIndex;
	
	UPROPERTY()
	FTimerHandle PreviewContentRefreshTH;
};
