// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetSettingItemBase.h"

#include "Components/TextBlock.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetSettingItemBase::UWidgetSettingItemBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetSettingItemBase::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);;
}

void UWidgetSettingItemBase::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	SetLabel(FText::GetEmpty());
	SetValue(FParameter());
}

void UWidgetSettingItemBase::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetSettingItemBase::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	if(InParams.IsValidIndex(0))
	{
		SetLabel(InParams[0].GetTextValue());
	}
}

void UWidgetSettingItemBase::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetSettingItemBase::OnDestroy()
{
	Super::OnDestroy();
}

FText UWidgetSettingItemBase::GetLabel() const
{
	return Txt_Label->GetText();
}

void UWidgetSettingItemBase::SetLabel(FText InText)
{
	Txt_Label->SetText(InText);
}

FParameter UWidgetSettingItemBase::GetValue() const
{
	return FParameter();
}

void UWidgetSettingItemBase::SetValue(const FParameter& InValue)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, InValue);
	}
	Refresh();
}
