// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/Preview/WidgetInventoryShopSlot.h"

UWidgetInventoryShopSlot::UWidgetInventoryShopSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventoryShopSlot::OnInitialize_Implementation(UUserWidgetBase* InOwner, FAbilityItem InItem)
{
	Super::OnInitialize_Implementation(InOwner, InItem);
}
