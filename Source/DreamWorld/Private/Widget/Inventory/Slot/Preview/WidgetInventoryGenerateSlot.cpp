// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/Preview/WidgetInventoryGenerateSlot.h"

UWidgetInventoryGenerateSlot::UWidgetInventoryGenerateSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventoryGenerateSlot::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	OnSlotSelected.Clear();
}

FReply UWidgetInventoryGenerateSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == FKey("LeftMouseButton"))
	{
		OnSlotSelected.Broadcast(this);
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWidgetInventoryGenerateSlot::OnInitialize_Implementation(UUserWidgetBase* InOwner, FAbilityItem InItem)
{
	Super::OnInitialize_Implementation(InOwner, InItem);
}
