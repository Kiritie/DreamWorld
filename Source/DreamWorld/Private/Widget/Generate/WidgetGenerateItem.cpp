// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Generate/WidgetGenerateItem.h"

#include "Widget/Generate/WidgetGeneratePanel.h"

UWidgetGenerateItem::UWidgetGenerateItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;
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

void UWidgetGenerateItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(UWidgetGeneratePanel* GeneratePanel = GetOwnerWidget<UWidgetGeneratePanel>())
	{
		GeneratePanel->OnGenerateItemSelected(this);
	}
}

void UWidgetGenerateItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(UWidgetGeneratePanel* GeneratePanel = GetOwnerWidget<UWidgetGeneratePanel>())
	{
		GeneratePanel->OnGenerateItemDeselected(this);
	}
}
