// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/Info/WidgetAbilityItemInfoBox.h"

#include "Widget/Item/Info/WidgetAbilityItemInfo.h"

UWidgetAbilityItemInfoBox::UWidgetAbilityItemInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ItemInfoBox");
	ParentName = FName("GameHUD");

	WidgetType = EWidgetType::Permanent;

	WidgetZOrder = 3;
}

void UWidgetAbilityItemInfoBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetAbilityItemInfoBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetAbilityItemInfoBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	TArray<FAbilityItem> Items;
	if(InParams.IsValidIndex(0))
	{
		Items = InParams[0].GetPointerValueRef<TArray<FAbilityItem>>();
	}
	
	TArray<UWidgetAbilityItemInfo*> ItemInfos = GetAllSubWidgets<UWidgetAbilityItemInfo>();
	for(int32 i = 0; i < ItemInfos.Num(); i++)
	{
		ItemInfos[i]->SetVisibility(ESlateVisibility::Collapsed);
		if(Items.IsValidIndex(i))
		{
			ItemInfos[i]->Init({ &Items[i] });
			ItemInfos[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UWidgetAbilityItemInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
