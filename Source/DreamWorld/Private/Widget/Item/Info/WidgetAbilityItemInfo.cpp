// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/Info/WidgetAbilityItemInfo.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

UWidgetAbilityItemInfo::UWidgetAbilityItemInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtName = nullptr;
	TxtRarity = nullptr;
	TxtType = nullptr;
	TxtLevel = nullptr;
	TxtDetail = nullptr;
	TxtErrorInfo = nullptr;
	TxtAbilityInfo = nullptr;
	
	Item = FAbilityItem::Empty;
}

void UWidgetAbilityItemInfo::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetAbilityItemInfo::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Item = InParams[0].GetPointerValueRef<FAbilityItem>();
	}
	Super::OnInitialize(InParams);
}

void UWidgetAbilityItemInfo::OnRefresh()
{
	Super::OnRefresh();

	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();
		TitleBox->SetVisibility(ItemData.GetItemType() != EAbilityItemType::Misc ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TxtName->SetText(ItemData.Name);
		TxtRarity->SetVisibility(ItemData.Rarity != EAbilityItemRarity::None ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TxtRarity->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumDisplayNameByValue(TEXT("/Script/WHFramework.EAbilityItemRarity"), (int32)ItemData.Rarity).ToString())));
		TxtType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumDisplayNameByValue(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), Item.Level)));
		TxtLevel->SetVisibility(Item.Level != 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TxtDetail->SetText(!ItemData.Detail.IsEmpty() ? ItemData.Detail : FText::FromString(TEXT("暂无描述")));
		TxtErrorInfo->SetText(FText::FromString(ItemData.GetItemErrorInfo(Item)));
		TxtAbilityInfo->SetText(FText::FromString(ItemData.GetItemAbilityInfo(Item.Level)));
	}
}

void UWidgetAbilityItemInfo::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	Item = FAbilityItem::Empty;
}
