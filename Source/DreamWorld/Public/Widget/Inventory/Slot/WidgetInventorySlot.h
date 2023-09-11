// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Inventory/Widget/WidgetInventorySlotBase.h"
#include "WidgetInventorySlot.generated.h"

/**
 * UI物品槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventorySlot : public UWidgetInventorySlotBase
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
	virtual void OnCreate_Implementation(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh_Implementation() override;

	virtual void OnActivated_Implementation() override;
	
	virtual void OnCanceled_Implementation() override;
		
protected:
	virtual void StartCooldown_Implementation() override;
			
	virtual void StopCooldown_Implementation() override;

	virtual void OnCooldown_Implementation() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetBorderColor(FLinearColor InColor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UBorder* Border;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UCanvasPanel* Content;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UImage* ImgIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UImage* ImgMask;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UTextBlock* TxtKeyCode;

	UPROPERTY()
	UMaterialInstanceDynamic* MaskMatInst;
};
