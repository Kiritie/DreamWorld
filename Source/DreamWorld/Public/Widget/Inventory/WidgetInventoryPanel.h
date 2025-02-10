// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widget/Inventory/WidgetInventory.h"
#include "WidgetInventoryPanel.generated.h"

class UCommonButtonBase;
class UCommonButtonGroup;
class UGridPanel;
class UWrapBox;
class UWidgetInventoryEquipSlot;

/**
 * ��ɫ���
 */
UCLASS()
class DREAMWORLD_API UWidgetInventoryPanel : public UWidgetInventory
{
	GENERATED_BODY()

public:
	UWidgetInventoryPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

protected:
	UFUNCTION()
	void OnGroupButtonSelected(UCommonButtonBase* AssociatedButton, int32 ButtonIndex);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UWrapBox* DefaultContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* LeftEquipContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UGridPanel* RightEquipContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UPanelWidget* WeaponGroupContent;

protected:
	UPROPERTY()
	UCommonButtonGroup* WeaponGroup;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHeadInfo(const FString& InHeadInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealthInfo(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetManaInfo(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStaminaInfo(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetAttackForce(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetAttackSpeed(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetAttackCritRate(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetAttackStealRate(const FString& InValue);
			
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetRepulseForce(const FString& InValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetDefendRate(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPhysicsRes(const FString& InValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetMagicRes(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetToughnessRate(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetMoveSpeed(const FString& InValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetSwimSpeed(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetFlySpeed(const FString& InValue);
};
