// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Item/WidgetInventoryItemInfoBox.h"
#include "Components/TextBlock.h"

UWidgetInventoryItemInfoBox::UWidgetInventoryItemInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryItemInfoBox");
	ParentName = FName("GameHUD");
	InputMode = EInputMode::None;

	WidgetType = EWidgetType::Permanent;
}

void UWidgetInventoryItemInfoBox::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetInventoryItemInfoBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetInventoryItemInfoBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
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

void UWidgetInventoryItemInfoBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}
