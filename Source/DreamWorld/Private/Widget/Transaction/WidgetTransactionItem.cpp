// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Transaction/WidgetTransactionItem.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widget/Transaction/WidgetTransactionPanel.h"

UWidgetTransactionItem::UWidgetTransactionItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	ImgIcon = nullptr;
	TxtName = nullptr;
	TxtType = nullptr;
	TxtLevel = nullptr;
	TxtCount = nullptr;
}

void UWidgetTransactionItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void UWidgetTransactionItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetTransactionItem::OnRefresh()
{
	Super::OnRefresh();

	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();

		ImgIcon->SetBrushResourceObject(ItemData.Icon);
		TxtName->SetText(ItemData.Name);
		TxtType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtLevel->SetText(FText::FromString(Item.Level != 0 ? FString::Printf(TEXT("Lv.%d"), Item.Level) : TEXT("")));
		TxtCount->SetText(FText::FromString(FString::FromInt(Item.Count)));
	}
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
