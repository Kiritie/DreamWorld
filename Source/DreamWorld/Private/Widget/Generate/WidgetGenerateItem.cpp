// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Generate/WidgetGenerateItem.h"

#include "Widget/Generate/WidgetGeneratePanel.h"

UWidgetGenerateItem::UWidgetGenerateItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetGenerateItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	GenerateItemData = FDWGenerateItemData();
}

void UWidgetGenerateItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(1))
	{
		GenerateItemData = InParams[1].GetPointerValueRef<FDWGenerateItemData>();
	}
	Super::OnInitialize(InParams);
}

void UWidgetGenerateItem::OnSelected_Implementation()
{
	Super::OnSelected_Implementation();

	if(UWidgetGeneratePanel* GeneratePanel = Cast<UWidgetGeneratePanel>(Owner))
	{
		GeneratePanel->OnGenerateSlotSelected(this);
	}
}

void UWidgetGenerateItem::OnUnSelected_Implementation()
{
	Super::OnUnSelected_Implementation();
}
