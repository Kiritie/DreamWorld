// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/WidgetAbilityItemInfoBox.h"

#include "Components/TextBlock.h"

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

	if(TextBlock_ItemName && InParams.IsValidIndex(0))
	{
		TextBlock_ItemName->SetText(InParams[0]);
	}

	if(TextBlock_ItemDetail && InParams.IsValidIndex(1))
	{
		TextBlock_ItemDetail->SetText(!InParams[1].GetTextValue().IsEmpty() ? InParams[1].GetTextValue() : FText::FromString(TEXT("暂无描述")));
	}
}

void UWidgetAbilityItemInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
