// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Transaction/WidgetTransactionItem.h"

#include "Widget/Transaction/WidgetTransactionPanel.h"

UWidgetTransactionItem::UWidgetTransactionItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;
}

void UWidgetTransactionItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void UWidgetTransactionItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetTransactionItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(UWidgetTransactionPanel* TransactionPanel = GetOwnerWidget<UWidgetTransactionPanel>())
	{
		TransactionPanel->OnTransactionItemSelected(this);
	}
}

void UWidgetTransactionItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(UWidgetTransactionPanel* TransactionPanel = GetOwnerWidget<UWidgetTransactionPanel>())
	{
		TransactionPanel->OnTransactionItemDeselected(this);
	}
}
