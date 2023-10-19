// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/WidgetAbilityItemInfoBox.h"

#include "Components/TextBlock.h"

UWidgetAbilityItemInfoBox::UWidgetAbilityItemInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ItemInfoBox");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::None;

	WidgetType = EWidgetType::Permanent;
}

void UWidgetAbilityItemInfoBox::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetAbilityItemInfoBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetAbilityItemInfoBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	if(TextBlock_ItemName && InParams.IsValidIndex(0))
	{
		TextBlock_ItemName->SetText(InParams[0].GetTextValue());
	}

	if(TextBlock_ItemDetail && InParams.IsValidIndex(1))
	{
		TextBlock_ItemDetail->SetText(!InParams[1].GetTextValue().IsEmpty() ? InParams[1].GetTextValue() : FText::FromString(TEXT("暂无描述")));
	}
}

void UWidgetAbilityItemInfoBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
