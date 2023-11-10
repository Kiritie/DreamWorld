// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetKeySettingItem.h"

#include "CommonButtonBase.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetButtonBase.h"
#include "Widget/Setting/Misc/WidgetPressAnyKeyPanel.h"
#include "Widget/Setting/Page/WidgetInputSettingPage.h"

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

	for(int32 i = 0; i < Btn_Values.Num(); i++)
	{
		Btn_Values[i]->OnClicked().AddUObject(this, &UWidgetKeySettingItem::OnValueButtonClicked, i);
	}
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

void UWidgetKeySettingItem::OnValueButtonClicked(int32 InIndex)
{
	UWidgetPressAnyKeyPanel* PressAnyKeyPanel = UWidgetModuleStatics::CreateUserWidget<UWidgetPressAnyKeyPanel>();
	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::OnKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::OnKeySelectionCanceled, PressAnyKeyPanel);
	PressAnyKeyPanel->Open({ InIndex });
}

void UWidgetKeySettingItem::OnKeySelected(FKey InKey, UWidgetPressAnyKeyPanel* InPressAnyKeyPanel)
{
	InPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	Btn_Values[InPressAnyKeyPanel->GetWidgetParams()[0].GetIntegerValue()]->SetButtonText(FText::FromString(InKey.ToString()));
	if(OnValuesChanged.IsBound())
	{
		OnValuesChanged.Broadcast(this, GetValues());
	}
	Refresh();
}

void UWidgetKeySettingItem::OnKeySelectionCanceled(UWidgetPressAnyKeyPanel* InPressAnyKeyPanel)
{
	InPressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

TArray<FParameter> UWidgetKeySettingItem::GetValues() const
{
	TArray<FParameter> Values;
	for(int32 i = 0; i < Btn_Values.Num(); i++)
	{
		Values.Add(Btn_Values[i]->GetButtonText().ToString());
	}
	return Values;
}

void UWidgetKeySettingItem::SetValues(const TArray<FParameter>& InValues)
{
	for(int32 i = 0; i < InValues.Num(); i++)
	{
		Btn_Values[i]->SetButtonText(FText::FromString(InValues[i].GetStringValue()));
	}
	Super::SetValues(InValues);
}
