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

void UWidgetBoolSettingItem::OnCreate_Implementation(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate_Implementation(InOwner, InParams);
	
	CheckBox_Value->OnCheckStateChanged.AddDynamic(this, &UWidgetBoolSettingItem::OnCheckBoxStateChanged);
}

void UWidgetBoolSettingItem::OnInitialize_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnInitialize_Implementation(InParams);
}

void UWidgetBoolSettingItem::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}

void UWidgetBoolSettingItem::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}

void UWidgetBoolSettingItem::OnCheckBoxStateChanged(bool bIsChecked)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, bIsChecked);
	}
	Refresh();
}

FParameter UWidgetBoolSettingItem::GetValue() const
{
	return CheckBox_Value->IsChecked();
}

void UWidgetBoolSettingItem::SetValue(const FParameter& InValue)
{
	CheckBox_Value->SetIsChecked(InValue.GetBooleanValue());
	Refresh();
}
