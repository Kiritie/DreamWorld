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

	if(InParams.IsValidIndex(1))
	{
		const FString EnumName = InParams[1].GetStringValue();
		for(int32 i = 0; i < UCommonBPLibrary::GetEnumItemNum(EnumName); i++)
		{
			ComboBox_Value->AddOption(UCommonBPLibrary::GetEnumValueDisplayName(EnumName, i).ToString());
		}
	}
}

void UWidgetEnumSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	ComboBox_Value->ClearOptions();
}

FParameter UWidgetEnumSettingItem::GetValue() const
{
	return ComboBox_Value->GetSelectedIndex();
}

void UWidgetEnumSettingItem::SetValue(const FParameter& InValue)
{
	ComboBox_Value->SetSelectedIndex(InValue.GetIntegerValue());
}
