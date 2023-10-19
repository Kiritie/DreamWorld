// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

#include "Components/Slider.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetFloatSettingItem::UWidgetFloatSettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetFloatSettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetFloatSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

FParameter UWidgetFloatSettingItem::GetValue() const
{
	return Slider_Value->GetValue();
}

void UWidgetFloatSettingItem::SetValue(const FParameter& InValue)
{
	Slider_Value->SetValue(InValue.GetFloatValue());
}
