// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Screen/UMG/UserWidgetBase.h"
#include "WidgetGeneratePanel.generated.h"

class UScrollBox;
class UWrapBox;
class UWidgetInventoryGenerateSlot;
class UWidgetInventoryPreviewSlot;
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UScrollBox* GenerateContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UWrapBox* PreviewContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UButton* BtnGenerate;

protected:
	TSubclassOf<UWidgetInventoryGenerateSlot> GenerateSlotClass;

	TSubclassOf<UWidgetInventoryPreviewSlot> PreviewSlotClass;

	TArray<UWidgetInventoryGenerateSlot*> GenerateSlots;
	
	TArray<UWidgetInventoryPreviewSlot*> PreviewSlots;

	UWidgetInventoryGenerateSlot* SelectedGenerateSlot;

public:
	virtual void OnCreate_Implementation(AActor* InOwner) override;

	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

	virtual void OnRefresh_Implementation() override;

	virtual void OnDestroy_Implementation(bool bRecovery = false) override;

protected:
	UFUNCTION()
	virtual void OnGenerateSlotSelected(UWidgetInventoryGenerateSlot* InSlot);

	UFUNCTION()
	virtual void OnGenerateButtonClicked();

public:
	UFUNCTION(BlueprintPure)
	bool GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData);
};
