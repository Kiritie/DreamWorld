// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Talent/WidgetTalentItem.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Widget/Talent/WidgetTalentContainer.h"
#include "Widget/Talent/WidgetTalentLine.h"
#include "Widget/Talent/WidgetTalentPanel.h"

UWidgetTalentItem::UWidgetTalentItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;
}

void UWidgetTalentItem::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTalentItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(1))
	{
		ParentItem = InParams[1].GetPointerValueRef<FAbilityItem>();
		if(ParentItem.IsValid())
		{
			TalentLine = UObjectPoolModuleStatics::SpawnObject<UWidgetTalentLine>(this, nullptr, TalentLineClass);
		}
	}
	Super::OnInitialize(InParams);
}

void UWidgetTalentItem::OnRefresh()
{
	Super::OnRefresh();

	if(Item.IsValid())
	{
		const auto& ItemData = GetItem().GetData();
		if(TxtLevel)
		{
			TxtLevel->SetText(FText::FromString(Item.Level < ItemData.MaxLevel ? FString::FromInt(Item.Level) : TEXT("Max")));
			TxtLevel->SetVisibility(Item.Level != 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
		if(ImgError)
		{
			ImgError->SetVisibility(ItemData.GetItemErrorInfo(Item).IsEmpty() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
	}

	SetIsSelectable(!ParentItem.IsValid() || ParentItem.Level == ParentItem.GetData().MaxLevel);
}

void UWidgetTalentItem::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	if(TalentLine)
	{
		TalentLine->Destroy();
	}
}

void UWidgetTalentItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTalentPanel* TalentPanel = GetOwnerWidget<UWidgetTalentPanel>())
		{
			TalentPanel->OnTalentItemSelected(this);
		}
	}
}

void UWidgetTalentItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTalentPanel* TalentPanel = GetOwnerWidget<UWidgetTalentPanel>())
		{
			TalentPanel->OnTalentItemDeselected(this);
		}
	}
}

void UWidgetTalentItem::OnAddToContainer(UWidgetTalentContainer* InTalentContainer)
{
	if(TalentLine)
	{
		if(UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(InTalentContainer->GetTalentContent()->AddChild(TalentLine)))
		{
			HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			HorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
			HorizontalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
		}
	}
	if(UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(InTalentContainer->GetTalentContent()->AddChild(this)))
	{
		HorizontalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
	}
}
