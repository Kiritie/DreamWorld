// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetVideoSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Video/DWVideoModule.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetEnumSettingItem.h"

UWidgetVideoSettingPage::UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VideoSettingPage");

	Title = FText::FromString(TEXT("视频"));
}

void UWidgetVideoSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetVideoSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	SettingItem_GlobalVideoQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("全局质量")), FString("/Script/WHFramework.EVideoQuality") }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	SettingItem_GlobalVideoQuality->OnValueChanged.AddDynamic(this, &UWidgetVideoSettingPage::OnSettingItemValueChange);
	AddSettingItem(SettingItem_GlobalVideoQuality, FText::FromString(TEXT("视频")));

	SettingItem_ViewDistanceQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("视野距离")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	SettingItem_ViewDistanceQuality->OnValueChanged.AddDynamic(this, &UWidgetVideoSettingPage::OnSettingItemValueChange);
	AddSettingItem(SettingItem_ViewDistanceQuality, FText::FromString(TEXT("视频")));

	SettingItem_ShadowQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("阴影质量")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	SettingItem_ShadowQuality->OnValueChanged.AddDynamic(this, &UWidgetVideoSettingPage::OnSettingItemValueChange);
	AddSettingItem(SettingItem_ShadowQuality, FText::FromString(TEXT("视频")));}

void UWidgetVideoSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	SettingItem_GlobalVideoQuality->SetValue((int32)ADWVideoModule::Get()->GetGlobalVideoQuality());
	SettingItem_ViewDistanceQuality->SetValue((int32)ADWVideoModule::Get()->GetViewDistanceQuality());
	SettingItem_ShadowQuality->SetValue((int32)ADWVideoModule::Get()->GetShadowQuality());
}

void UWidgetVideoSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();

	ADWVideoModule::Get()->SetGlobalVideoQuality((EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetViewDistanceQuality((EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetShadowQuality((EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue(), false);

	ADWVideoModule::Get()->ApplyVideoQualitySettings();
}

void UWidgetVideoSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();

	SettingItem_GlobalVideoQuality->SetValue((int32)GetDefaultVideoData().GlobalVideoQuality);
	SettingItem_ViewDistanceQuality->SetValue((int32)GetDefaultVideoData().ViewDistanceQuality);
	SettingItem_ShadowQuality->SetValue((int32)GetDefaultVideoData().ShadowQuality);
}

void UWidgetVideoSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetVideoSettingPage::CanApply_Implementation() const
{
	return ADWVideoModule::Get()->GetGlobalVideoQuality() != (EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetViewDistanceQuality() != (EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetShadowQuality() != (EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue();
}

bool UWidgetVideoSettingPage::CanReset_Implementation() const
{
	return ADWVideoModule::Get()->GetGlobalVideoQuality() != GetDefaultVideoData().GlobalVideoQuality ||
		ADWVideoModule::Get()->GetViewDistanceQuality() != GetDefaultVideoData().ViewDistanceQuality ||
		ADWVideoModule::Get()->GetShadowQuality() != GetDefaultVideoData().ShadowQuality;
}

void UWidgetVideoSettingPage::OnSettingItemValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	if(InSettingItem == SettingItem_GlobalVideoQuality)
	{
		if(InValue != (int32)EVideoQuality::Custom)
		{
			SettingItem_ViewDistanceQuality->SetValue(InValue.GetIntegerValue());
			SettingItem_ShadowQuality->SetValue(InValue.GetIntegerValue());
		}
	}
	else
	{
		if(InValue != SettingItem_GlobalVideoQuality->GetValue())
		{
			SettingItem_GlobalVideoQuality->SetValue((int32)EVideoQuality::Custom);
		}
	}
}

FDWVideoModuleSaveData& UWidgetVideoSettingPage::GetDefaultVideoData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().VideoData;
}
