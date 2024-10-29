// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Inventory/WidgetInventory.h"
#include "WidgetInventoryBar.generated.h"

class USizeBox;
class UGridPanel;
class UWidgetInventorySkillSlot;
class ADWPlayerCharacter;

/**
 * ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UWidgetInventoryBar : public UWidgetInventory
{
	GENERATED_BODY()

public:
	UWidgetInventoryBar(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

	UFUNCTION()
	virtual void OnInventorySlotSelected(UAbilityInventorySlotBase* InInventorySlot);

public:
	UFUNCTION(BlueprintCallable)
	void PrevInventorySlot();

	UFUNCTION(BlueprintCallable)
	void NextInventorySlot();
	
	UFUNCTION(BlueprintCallable)
	void SelectInventorySlot(int32 InSlotIndex, bool bRefreshInventory = true);
	
	UFUNCTION(BlueprintCallable)
	void SetSkillBoxVisible(bool bValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* ShortcutContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* AuxiliaryContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* LeftSkillContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* RightSkillContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	USizeBox* LeftSkillBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	USizeBox* RightSkillBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	USizeBox* SelectBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SelectedSlotIndex;

public:
	UFUNCTION(BlueprintPure)
	UAbilityInventorySlotBase* GetSelectedSlot() const;
	
	UFUNCTION(BlueprintPure)
	FAbilityItem GetSelectedItem() const;

	UFUNCTION(BlueprintPure)
	int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }
};
