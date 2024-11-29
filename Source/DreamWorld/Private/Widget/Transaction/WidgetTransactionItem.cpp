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
	TxtRarity = nullptr;
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

		TxtRarity->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemRarity"), (int32)ItemData.Rarity).ToString())));
		TxtRarity->SetVisibility(ItemData.Rarity != EAbilityItemRarity::None ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TxtType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), Item.Level)));
		TxtLevel->SetVisibility(Item.Level != 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TxtCount->SetText(FText::FromString(Item.Count > 1 ? FString::FromInt(Item.Count) : TEXT("")));
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
