// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
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

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual void OnInitialize(UInventorySlot* InOwnerSlot) override;

protected:
	virtual void OnRefresh() override;

	virtual void OnActivated() override;
	
	virtual void OnCanceled() override;
		
protected:
	virtual void StartCooldown() override;
			
	virtual void StopCooldown() override;

	virtual void OnCooldown() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText KeyCode;

	UPROPERTY()
	UMaterialInstanceDynamic* MaskMatInst;

public:
	FText GetKeyCode() const { return KeyCode; }
	
	void SetKeyCode(const FText& InKeyCode) { this->KeyCode = InKeyCode; }
};
