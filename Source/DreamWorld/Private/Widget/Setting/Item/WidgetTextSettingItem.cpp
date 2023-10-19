// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetTextSettingItem.h"

#include "Components/EditableTextBox.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetTextSettingItem::UWidgetTextSettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetTextSettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);

	if(InParams.IsValidIndex(1))
	{
		TxtBox_Value->KeyboardType = InParams[1].GetPointerValueRef<EVirtualKeyboardType::Type>();
	}
}

void UWidgetTextSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	TxtBox_Value->KeyboardType = EVirtualKeyboardType::Default;

	Super::OnDespawn_Implementation(bRecovery);
}

FParameter UWidgetTextSettingItem::GetValue() const
{
	FParameter Value;
	switch (TxtBox_Value->KeyboardType)
	{
		case EVirtualKeyboardType::Default:
		{
			Value = TxtBox_Value->GetText().ToString();
			break;
		}
		case EVirtualKeyboardType::Number:
		{
			Value = FCString::Atof(*TxtBox_Value->GetText().ToString());
			break;
		}
		default: break;
	}
	return Value;
}

void UWidgetTextSettingItem::SetValue(const FParameter& InValue)
{
	FText Text;
	switch (TxtBox_Value->KeyboardType)
	{
		case EVirtualKeyboardType::Default:
		{
			Text = FText::FromString(InValue.GetStringValue());
			break;
		}
		case EVirtualKeyboardType::Number:
		{
			Text = FText::FromString(FString::Printf(TEXT("%3f"), InValue.GetFloatValue()));
			break;
		}
		default: break;
	}
	TxtBox_Value->SetText(Text);
}
