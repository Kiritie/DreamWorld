// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetAudioSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

UWidgetAudioSettingPage::UWidgetAudioSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("AudioSettingPage");

	Title = FText::FromString(TEXT("音频"));
}

void UWidgetAudioSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetAudioSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	SettingItem_GlobalSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_GlobalSoundVolume, FText::FromString(TEXT("全局")));

	SettingItem_BackgroundSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_BackgroundSoundVolume, FText::FromString(TEXT("背景")));

	SettingItem_EnvironmentSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EnvironmentSoundVolume, FText::FromString(TEXT("环境")));

	SettingItem_EffectSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EffectSoundVolume, FText::FromString(TEXT("音效")));
}

void UWidgetAudioSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GlobalSoundVolume->SetValue(UAudioModuleBPLibrary::GetGlobalSoundVolume());
	SettingItem_BackgroundSoundVolume->SetValue(UAudioModuleBPLibrary::GetBackgroundSoundVolume());
	SettingItem_EnvironmentSoundVolume->SetValue(UAudioModuleBPLibrary::GetEnvironmentSoundVolume());
	SettingItem_EffectSoundVolume->SetValue(UAudioModuleBPLibrary::GetEffectSoundVolume());
}

void UWidgetAudioSettingPage::OnApply()
{
	Super::OnApply();

	UAudioModuleBPLibrary::SetGlobalSoundVolume(SettingItem_GlobalSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetBackgroundSoundVolume(SettingItem_BackgroundSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetEnvironmentSoundVolume(SettingItem_EnvironmentSoundVolume->GetValue().GetFloatValue());
	UAudioModuleBPLibrary::SetEffectSoundVolume(SettingItem_EffectSoundVolume->GetValue().GetFloatValue());
}

void UWidgetAudioSettingPage::OnReset()
{
	Super::OnReset();

	SettingItem_GlobalSoundVolume->SetValue(GetDefaultAudioData().GlobalSoundParams.Volume);
	SettingItem_BackgroundSoundVolume->SetValue(GetDefaultAudioData().BackgroundSoundParams.Volume);
	SettingItem_EnvironmentSoundVolume->SetValue(GetDefaultAudioData().EnvironmentSoundParams.Volume);
	SettingItem_EffectSoundVolume->SetValue(GetDefaultAudioData().EffectSoundParams.Volume);
}

void UWidgetAudioSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
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
	return UAudioModuleBPLibrary::GetGlobalSoundVolume() != GetDefaultAudioData().GlobalSoundParams.Volume ||
		UAudioModuleBPLibrary::GetBackgroundSoundVolume() != GetDefaultAudioData().BackgroundSoundParams.Volume ||
		UAudioModuleBPLibrary::GetEnvironmentSoundVolume() != GetDefaultAudioData().EnvironmentSoundParams.Volume ||
		UAudioModuleBPLibrary::GetEffectSoundVolume() != GetDefaultAudioData().EffectSoundParams.Volume;
}

FDWAudioModuleSaveData& UWidgetAudioSettingPage::GetDefaultAudioData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().AudioData;
}
