// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetAudioSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

UWidgetAudioSettingPage::UWidgetAudioSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("AudioSettingPage");

	Title = FText::FromString(TEXT("音频"));

	DefaultAudioData = FDWAudioModuleSaveData();
}

void UWidgetAudioSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetAudioSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	SettingItem_GlobalSoundVolume = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")) }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_GlobalSoundVolume, FText::FromString(TEXT("全局")));

	SettingItem_BackgroundSoundVolume = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")) }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_BackgroundSoundVolume, FText::FromString(TEXT("背景")));

	SettingItem_EnvironmentSoundVolume = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")) }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EnvironmentSoundVolume, FText::FromString(TEXT("环境")));

	SettingItem_EffectSoundVolume = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")) }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EffectSoundVolume, FText::FromString(TEXT("音效")));
}

void UWidgetAudioSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	SettingItem_GlobalSoundVolume->SetValue(UAudioModuleBPLibrary::GetGlobalSoundVolume());
	SettingItem_BackgroundSoundVolume->SetValue(UAudioModuleBPLibrary::GetBackgroundSoundVolume());
	SettingItem_EnvironmentSoundVolume->SetValue(UAudioModuleBPLibrary::GetEnvironmentSoundVolume());
	SettingItem_EffectSoundVolume->SetValue(UAudioModuleBPLibrary::GetEffectSoundVolume());
}

void UWidgetAudioSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();

	UAudioModuleBPLibrary::SetGlobalSoundVolume(SettingItem_GlobalSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetBackgroundSoundVolume(SettingItem_BackgroundSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetEnvironmentSoundVolume(SettingItem_EnvironmentSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetEffectSoundVolume(SettingItem_EffectSoundVolume->GetValue().GetFloatValue());
}

void UWidgetAudioSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();

	SettingItem_GlobalSoundVolume->SetValue(DefaultAudioData.GlobalSoundParams.Volume);
	SettingItem_BackgroundSoundVolume->SetValue(DefaultAudioData.BackgroundSoundParams.Volume);
	SettingItem_EnvironmentSoundVolume->SetValue(DefaultAudioData.EnvironmentSoundParams.Volume);
	SettingItem_EffectSoundVolume->SetValue(DefaultAudioData.EffectSoundParams.Volume);
}

void UWidgetAudioSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetAudioSettingPage::CanApply_Implementation() const
{
	return UAudioModuleBPLibrary::GetGlobalSoundVolume() != SettingItem_GlobalSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleBPLibrary::GetBackgroundSoundVolume() != SettingItem_BackgroundSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleBPLibrary::GetEnvironmentSoundVolume() != SettingItem_EnvironmentSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleBPLibrary::GetEffectSoundVolume() != SettingItem_EffectSoundVolume->GetValue().GetFloatValue();
}

bool UWidgetAudioSettingPage::CanReset_Implementation() const
{
	return UAudioModuleBPLibrary::GetGlobalSoundVolume() != DefaultAudioData.GlobalSoundParams.Volume ||
		UAudioModuleBPLibrary::GetBackgroundSoundVolume() != DefaultAudioData.BackgroundSoundParams.Volume ||
		UAudioModuleBPLibrary::GetEnvironmentSoundVolume() != DefaultAudioData.EnvironmentSoundParams.Volume ||
		UAudioModuleBPLibrary::GetEffectSoundVolume() != DefaultAudioData.EffectSoundParams.Volume;
}
