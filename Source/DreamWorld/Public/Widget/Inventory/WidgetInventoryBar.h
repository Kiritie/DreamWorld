// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Widget/Inventory/WidgetInventory.h"
#include "WidgetInventoryBar.generated.h"

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UGridPanel* ShortcutContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UGridPanel* AuxiliaryContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UGridPanel* LeftSkillContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class UGridPanel* RightSkillContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class USizeBox* LeftSkillBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class USizeBox* RightSkillBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	class USizeBox* SelectBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	int32 SelectedSlotIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UWidgetInventoryShortcutSlot> ShortcutSlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UWidgetInventoryAuxiliarySlot> AuxiliarySlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UWidgetInventorySkillSlot> SkillSlotClass;

public:
	virtual void OnInitialize_Implementation(AActor* InOwner) override;

	virtual void OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose_Implementation(bool bInstant) override;

	virtual void OnRefresh_Implementation() override;

	UFUNCTION()
	virtual void OnInventorySlotSelected(UInventorySlot* InInventorySlot);

public:
	UFUNCTION(BlueprintCallable)
	void PrevInventorySlot();

	UFUNCTION(BlueprintCallable)
	void NextInventorySlot();
	
	UFUNCTION(BlueprintCallable)
	void SelectInventorySlot(int32 InSlotIndex, bool bRefreshInventory = true);
	
	UFUNCTION(BlueprintCallable)
	void SetSkillBoxVisible(bool bValue);

public:
	UFUNCTION(BlueprintPure)
	UInventorySlot* GetSelectedSlot() const;
	
	UFUNCTION(BlueprintPure)
	FAbilityItem GetSelectedItem() const;

	UFUNCTION(BlueprintPure)
	int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }
};
