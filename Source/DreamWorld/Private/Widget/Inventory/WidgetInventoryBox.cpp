// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBox.h"

UWidgetInventoryBox::UWidgetInventoryBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryBox");
	WidgetType = EWidgetType::SemiPermanent;
	InputMode = EInputMode::GameAndUI;
}

void UWidgetInventoryBox::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetInventoryBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetInventoryBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}
