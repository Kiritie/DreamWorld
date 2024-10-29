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

	TArray<UWidgetAbilityItemInfo*> ItemInfos = GetAllSubWidgets<UWidgetAbilityItemInfo>();
	for(int32 i = 0; i < ItemInfos.Num(); i++)
	{
		ItemInfos[i]->SetVisibility(ESlateVisibility::Collapsed);
		if(InParams.IsValidIndex(i))
		{
			ItemInfos[i]->Init(TArray{ InParams[i] });
			ItemInfos[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UWidgetAbilityItemInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
