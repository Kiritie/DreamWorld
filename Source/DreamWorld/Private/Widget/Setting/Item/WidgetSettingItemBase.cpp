// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetSettingItemBase.h"

#include "Components/TextBlock.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetSettingItemBase::UWidgetSettingItemBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetSettingItemBase::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		SetLabel(InParams[0].GetTextValue());
	}
}

void UWidgetSettingItemBase::OnDespawn_Implementation(bool bRecovery)
{
	SetLabel(FText::GetEmpty());
	SetValue(FParameter());
}

FText UWidgetSettingItemBase::GetLabel() const
{
	return Txt_Label->GetText();
}

void UWidgetSettingItemBase::SetLabel(FText InText)
{
	Txt_Label->SetText(InText);
}

FParameter UWidgetSettingItemBase::GetValue() const
{
	return FParameter();
}

void UWidgetSettingItemBase::SetValue(const FParameter& InValue)
{
}
