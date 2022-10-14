// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Blueprint/UserWidget.h"
#include "ObjectPool/ObjectPoolInterface.h"
#include "WidgetInventoryPreviewSlot.generated.h"

/**
 * UI构建槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetInventoryPreviewSlot : public UUserWidget, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:
	UWidgetInventoryPreviewSlot(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidgetBase* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAbilityItem Item;

public:
	virtual int32 GetLimit_Implementation() const override { return 1000; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;
		
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnInitialize(UUserWidgetBase* InOwner, FAbilityItem InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnRefresh();

public:
	FAbilityItem GetItem() const { return Item; }
};
