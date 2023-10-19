// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetBoolSettingItem.h"

#include "Components/CheckBox.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetBoolSettingItem::UWidgetBoolSettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetBoolSettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetBoolSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

FParameter UWidgetBoolSettingItem::GetValue() const
{
	return CheckBox_Value->IsChecked();
}

void UWidgetBoolSettingItem::SetValue(const FParameter& InValue)
{
	CheckBox_Value->SetIsChecked(InValue.GetBooleanValue());
}
