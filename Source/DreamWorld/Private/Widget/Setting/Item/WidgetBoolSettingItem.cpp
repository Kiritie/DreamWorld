// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetBoolSettingItem.h"

#include "Components/CheckBox.h"
#include "Widget/WidgetModuleStatics.h"

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

void UWidgetBoolSettingItem::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	CheckBox_Value->OnCheckStateChanged.AddDynamic(this, &UWidgetBoolSettingItem::OnCheckBoxStateChanged);
}

void UWidgetBoolSettingItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetBoolSettingItem::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetBoolSettingItem::OnDestroy()
{
	Super::OnDestroy();
}

void UWidgetBoolSettingItem::OnCheckBoxStateChanged(bool bIsChecked)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, GetValue());
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
	Super::SetValue(InValue);
}
