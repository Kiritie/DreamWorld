// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/Info/WidgetAbilityItemInfo.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Components/TextBlock.h"

UWidgetAbilityItemInfo::UWidgetAbilityItemInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtItemName = nullptr;
	TxtItemType = nullptr;
	TxtItemLevel = nullptr;
	TxtItemDetail = nullptr;
}

void UWidgetAbilityItemInfo::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetAbilityItemInfo::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	if(InParams.IsValidIndex(0))
	{
		Item = InParams[0].GetPointerValueRef<FAbilityItem>();
	}

	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();

		TxtItemName->SetText(ItemData.Name);
		TxtItemType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtItemLevel->SetText(FText::FromString(Item.Level != 0 ? FString::Printf(TEXT("Lv.%d"), Item.Level) : TEXT("")));
		TxtItemDetail->SetText(!ItemData.Detail.IsEmpty() ? ItemData.Detail : FText::FromString(TEXT("暂无描述")));
	}
}

void UWidgetAbilityItemInfo::OnRefresh()
{
	Super::OnRefresh();
}
