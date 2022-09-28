// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInventorySlot.generated.h"

class UWidgetInventory;
class UInventorySlot;

/**
 * UI物品槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventorySlot : public UUserWidget
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

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION()
	virtual void OnInitialize(UInventorySlot* InOwnerSlot);

protected:
	UFUNCTION()
	virtual void OnRefresh();

	UFUNCTION()
	virtual void OnActivated();
	
	UFUNCTION()
	virtual void OnCanceled();
		
protected:
	UFUNCTION()
	virtual void StartCooldown();
			
	UFUNCTION()
	virtual void StopCooldown();

	UFUNCTION()
	virtual void OnCooldown();

public:
	UFUNCTION(BlueprintCallable)
	virtual void SplitItem(int InCount = -1);
	
	UFUNCTION(BlueprintCallable)
	virtual void MoveItem(int InCount = -1);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem(int InCount = -1);

	UFUNCTION(BlueprintCallable)
	virtual void DiscardItem(int InCount = -1);

	UFUNCTION(BlueprintCallable)
	virtual void SetBorderColor(FLinearColor InColor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	UInventorySlot* OwnerSlot;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText KeyCode;

	FTimerHandle CooldownTimerHandle;

public:
	FText GetKeyCode() const { return KeyCode; }
	
	void SetKeyCode(const FText& InKeyCode) { this->KeyCode = InKeyCode; }

protected:
	UPROPERTY()
	UMaterialInstanceDynamic* MaskMatInst;

public:
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	UFUNCTION(BlueprintPure)
	bool IsCooldowning() const;

	UFUNCTION(BlueprintPure)
	FAbilityItem& GetItem() const;

	UFUNCTION(BlueprintPure)
	UInventorySlot* GetOwnerSlot() const { return OwnerSlot; }

	UFUNCTION(BlueprintPure)
	UInventory* GetInventory() const;
};
