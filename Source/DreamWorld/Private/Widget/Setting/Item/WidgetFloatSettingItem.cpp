// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetFloatSettingItem::UWidgetFloatSettingItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MinValue = 0.f;
	MaxValue = 1.f;

	DecimalNum = 2;
	ScaleFactor = 1.f;
}

void UWidgetFloatSettingItem::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetFloatSettingItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	MinValue = 0.f;
	MaxValue = 1.f;

	DecimalNum = 2;
	ScaleFactor = 1.f;
}

void UWidgetFloatSettingItem::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	Slider_Value->OnValueChanged.AddDynamic(this, &UWidgetFloatSettingItem::OnSliderValueChanged);
}

void UWidgetFloatSettingItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(1))
	{
		MinValue = InParams[1].GetFloatValue();
	}

	if(InParams.IsValidIndex(2))
	{
		MaxValue = InParams[2].GetFloatValue();
	}

	if(InParams.IsValidIndex(3))
	{
		DecimalNum = InParams[3].GetIntegerValue();
	}

	if(InParams.IsValidIndex(4))
	{
		ScaleFactor = InParams[4].GetFloatValue();
	}

	Super::OnInitialize(InParams);
}

void UWidgetFloatSettingItem::OnRefresh()
{
	Super::OnRefresh();

	if(Txt_MinValue)
	{
		Txt_MinValue->SetText(UKismetTextLibrary::Conv_DoubleToText(MinValue * ScaleFactor, ERoundingMode::HalfToEven, false, true, 1, 324, 0, DecimalNum));
	}
	if(Txt_MaxValue)
	{
		Txt_MaxValue->SetText(UKismetTextLibrary::Conv_DoubleToText(MaxValue * ScaleFactor, ERoundingMode::HalfToEven, false, true, 1, 324, 0, DecimalNum));
	}
	if(Txt_CurrentValue)
	{
		Txt_CurrentValue->SetText(UKismetTextLibrary::Conv_DoubleToText(GetValue().GetFloatValue() * ScaleFactor, ERoundingMode::HalfToEven, false, true, 1, 324, 0, DecimalNum));
	}
}

void UWidgetFloatSettingItem::OnDestroy()
{
	Super::OnDestroy();
}

void UWidgetFloatSettingItem::OnSliderValueChanged(float InValue)
{
	if(OnValueChanged.IsBound())
	{
		OnValueChanged.Broadcast(this, GetValue());
	}
	Refresh();
}

FParameter UWidgetFloatSettingItem::GetValue() const
{
	return FMath::Lerp(MinValue, MaxValue, Slider_Value->GetValue());
}

void UWidgetFloatSettingItem::SetValue(const FParameter& InValue)
{
	Slider_Value->SetValue((InValue.GetFloatValue() - MinValue) / (MaxValue - MinValue));
	Super::SetValue(InValue);
}
