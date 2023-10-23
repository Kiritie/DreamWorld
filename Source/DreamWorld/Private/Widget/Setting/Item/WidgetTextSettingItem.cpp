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
}

void UWidgetTextSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	TxtBox_Value->KeyboardType = EVirtualKeyboardType::Default;

	Super::OnDespawn_Implementation(bRecovery);
}

void UWidgetTextSettingItem::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	TxtBox_Value->OnTextChanged.AddDynamic(this, &UWidgetTextSettingItem::OnTextBoxContentChanged);
}

void UWidgetTextSettingItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	if(InParams.IsValidIndex(1))
	{
		TxtBox_Value->KeyboardType = InParams[1].GetPointerValueRef<EVirtualKeyboardType::Type>();
	}
}

void UWidgetTextSettingItem::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetTextSettingItem::OnDestroy()
{
	Super::OnDestroy();
}

void UWidgetTextSettingItem::OnTextBoxContentChanged(const FText& InText)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, GetValue());
	}
	Refresh();
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
			Text = FText::FromString(FString::Printf(TEXT("%0.2f"), InValue.GetFloatValue()));
			break;
		}
		default: break;
	}
	TxtBox_Value->SetText(Text);
	Super::SetValue(InValue);
}
