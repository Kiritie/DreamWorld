// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/Info/WidgetAbilityItemInfo.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Components/TextBlock.h"

UWidgetAbilityItemInfo::UWidgetAbilityItemInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtName = nullptr;
	TxtType = nullptr;
	TxtLevel = nullptr;
	TxtDetail = nullptr;
	TxtAttribute = nullptr;
}

void UWidgetAbilityItemInfo::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetAbilityItemInfo::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetAbilityItemInfo::OnRefresh()
{
	Super::OnRefresh();

	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();
		TxtName->SetText(ItemData.Name);
		TxtType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtLevel->SetText(FText::FromString(Item.Level != 0 ? FString::Printf(TEXT("Lv.%d"), Item.Level) : TEXT("")));
		TxtDetail->SetText(!ItemData.Detail.IsEmpty() ? ItemData.Detail : FText::FromString(TEXT("暂无描述")));
		TxtAttribute->SetText(ItemData.GetItemAttributeInfo(Item.Level));
	}
}
