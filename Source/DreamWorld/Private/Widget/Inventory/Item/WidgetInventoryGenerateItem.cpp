// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Item/WidgetInventoryGenerateItem.h"

#include "Widget/WidgetGeneratePanel.h"

UWidgetInventoryGenerateItem::UWidgetInventoryGenerateItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetInventoryGenerateItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	GenerateItemData = FDWGenerateItemData();
}

void UWidgetInventoryGenerateItem::OnInitialize_Implementation(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(1))
	{
		GenerateItemData = InParams[1].GetPointerValueRef<FDWGenerateItemData>();
	}
	Super::OnInitialize_Implementation(InParams);
}

void UWidgetInventoryGenerateItem::OnSelected_Implementation()
{
	Super::OnSelected_Implementation();

	if(UWidgetGeneratePanel* GeneratePanel = Cast<UWidgetGeneratePanel>(Owner))
	{
		GeneratePanel->OnGenerateSlotSelected(this);
	}
}

void UWidgetInventoryGenerateItem::OnUnSelected_Implementation()
{
	Super::OnUnSelected_Implementation();
}
