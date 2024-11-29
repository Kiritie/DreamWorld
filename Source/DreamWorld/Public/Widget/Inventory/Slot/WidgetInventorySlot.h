// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Widget/WidgetAbilityInventorySlotBase.h"
#include "WidgetInventorySlot.generated.h"

class UWidgetKeyTipsItemBase;
class UTextBlock;
class UCanvasPanel;
/**
 * UI物品槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventorySlot : public UWidgetAbilityInventorySlotBase
{
	GENERATED_BODY()

public:
	UWidgetInventorySlot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual void OnTick_Implementation(float DeltaSeconds) override;

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnActivated_Implementation() override;
	
	virtual void OnDeactived_Implementation() override;
		
protected:
	virtual void StartCooldown_Implementation() override;
			
	virtual void StopCooldown_Implementation() override;

	virtual void OnCooldown_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCanvasPanel* Content;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UImage* ImgIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UImage* ImgMask;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UImage* ImgError;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWidgetKeyTipsItemBase* KeyTipsItem;

	UPROPERTY()
	UMaterialInstanceDynamic* MaskMatInst;
};
