// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetSettingItemCategory.h"

#include "Components/TextBlock.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetSettingItemCategory::UWidgetSettingItemCategory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetSettingItemCategory::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		SetCategory(InParams[0].GetTextValue());
	}
}

void UWidgetSettingItemCategory::OnDespawn_Implementation(bool bRecovery)
{
	SetCategory(FText::GetEmpty());
}

FText UWidgetSettingItemCategory::GetCategory() const
{
	return Txt_Category->GetText();
}

void UWidgetSettingItemCategory::SetCategory(FText InText)
{
	Txt_Category->SetText(InText);
}
