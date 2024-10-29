// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Generate/WidgetGenerateItem.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Common/CommonStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widget/Generate/WidgetGeneratePanel.h"

UWidgetGenerateItem::UWidgetGenerateItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	ImgIcon = nullptr;
	TxtName = nullptr;
	TxtType = nullptr;
	TxtLevel = nullptr;
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

void UWidgetGenerateItem::OnRefresh()
{
	Super::OnRefresh();

	if(Item.IsValid())
	{
		const auto& ItemData = Item.GetData();

		ImgIcon->SetBrushResourceObject(ItemData.Icon);
		TxtName->SetText(ItemData.Name);
		TxtType->SetText(FText::FromString(FString::Printf(TEXT("[%s]"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)ItemData.GetItemType()).ToString())));
		TxtLevel->SetText(FText::FromString(Item.Level != 0 ? FString::Printf(TEXT("Lv.%d"), Item.Level) : TEXT("")));
	}
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
