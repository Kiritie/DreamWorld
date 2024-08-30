// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Common/WidgetUIMask.h"

#include "Ability/Inventory/Widget/WidgetInventorySlotBase.h"
#include "Blueprint/DragDropOperation.h"

UWidgetUIMask::UWidgetUIMask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_UIMask");
}

void UWidgetUIMask::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetUIMask::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetUIMask::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetUIMask::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetUIMask::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const auto PayloadSlot = Cast<UWidgetInventorySlotBase>(InOperation->Payload);
	if (PayloadSlot && !PayloadSlot->IsEmpty())
	{
		PayloadSlot->DiscardItem();
		return true;
	}
	return false;
}
