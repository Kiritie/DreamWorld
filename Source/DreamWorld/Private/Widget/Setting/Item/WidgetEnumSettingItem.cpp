// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetEnumSettingItem.h"

#include "Common/CommonBPLibrary.h"
#include "Components/ComboBoxString.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetEnumSettingItem::UWidgetEnumSettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetEnumSettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetEnumSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	ComboBox_Value->ClearOptions();
}

void UWidgetEnumSettingItem::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	ComboBox_Value->OnSelectionChanged.AddDynamic(this, &UWidgetEnumSettingItem::OnComboBoxSelectionChanged);
}

void UWidgetEnumSettingItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	FString EnumName;
	int32 EnumMaxNum = -1;

	if(InParams.IsValidIndex(1))
	{
		EnumName = InParams[1].GetStringValue();
	}
	if(InParams.IsValidIndex(2))
	{
		EnumMaxNum = InParams[2].GetIntegerValue();
	}
	
	if(!EnumName.IsEmpty())
	{
		for(int32 i = 0; i < (EnumMaxNum != -1 ? EnumMaxNum : UCommonBPLibrary::GetEnumItemNum(EnumName)); i++)
		{
			ComboBox_Value->AddOption(UCommonBPLibrary::GetEnumValueDisplayName(EnumName, i).ToString());
		}
	}
}

void UWidgetEnumSettingItem::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetEnumSettingItem::OnDestroy()
{
	Super::OnDestroy();
}

void UWidgetEnumSettingItem::OnComboBoxSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, GetValue());
	}
	Refresh();
}

FParameter UWidgetEnumSettingItem::GetValue() const
{
	return ComboBox_Value->GetSelectedIndex();
}

void UWidgetEnumSettingItem::SetValue(const FParameter& InValue)
{
	ComboBox_Value->SetSelectedIndex(InValue.GetIntegerValue());
	Super::SetValue(InValue);
}
