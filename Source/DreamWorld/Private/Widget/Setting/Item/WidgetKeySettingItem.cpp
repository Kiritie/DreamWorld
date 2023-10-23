// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetKeySettingItem.h"

#include "CommonButtonBase.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Common/WidgetButtonBase.h"
#include "Widget/Setting/Misc/WidgetPressAnyKeyPanel.h"

UWidgetKeySettingItem::UWidgetKeySettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetKeySettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetKeySettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void UWidgetKeySettingItem::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	Btn_Value->OnClicked().AddUObject(this, &UWidgetKeySettingItem::OnValueButtonClicked);
}

void UWidgetKeySettingItem::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetKeySettingItem::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetKeySettingItem::OnDestroy()
{
	Super::OnDestroy();
}

void UWidgetKeySettingItem::OnValueButtonClicked()
{
	UWidgetPressAnyKeyPanel* PressAnyKeyPanel = UWidgetModuleBPLibrary::CreateUserWidget<UWidgetPressAnyKeyPanel>();
	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::OnKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::OnKeySelectionCanceled, PressAnyKeyPanel);
	PressAnyKeyPanel->Open();
}

void UWidgetKeySettingItem::OnKeySelected(FKey InKey, UWidgetPressAnyKeyPanel* InPressAnyKeyPanel)
{
	InPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	Btn_Value->SetButtonText(FText::FromString(InKey.ToString()));
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, GetValue());
	}
	Refresh();
}

void UWidgetKeySettingItem::OnKeySelectionCanceled(UWidgetPressAnyKeyPanel* InPressAnyKeyPanel)
{
	InPressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

FParameter UWidgetKeySettingItem::GetValue() const
{
	return Btn_Value->GetButtonText().ToString();
}

void UWidgetKeySettingItem::SetValue(const FParameter& InValue)
{
	Btn_Value->SetButtonText(FText::FromString(InValue.GetStringValue()));
	Super::SetValue(InValue);
}
