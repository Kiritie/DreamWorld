// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetAudioSettingPage.h"

#include "Asset/AssetModuleStatics.h"
#include "Audio/AudioModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleStatics.h"
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

	SettingItem_GlobalSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_GlobalSoundVolume, FText::FromString(TEXT("全局")));

	SettingItem_BackgroundSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_BackgroundSoundVolume, FText::FromString(TEXT("背景")));

	SettingItem_EnvironmentSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EnvironmentSoundVolume, FText::FromString(TEXT("环境")));

	SettingItem_EffectSoundVolume = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("音量大小")), 0.f, 1.f, 0, 100.f }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_EffectSoundVolume, FText::FromString(TEXT("音效")));
}

void UWidgetAudioSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GlobalSoundVolume->SetValue(UAudioModuleStatics::GetGlobalSoundVolume());
	SettingItem_BackgroundSoundVolume->SetValue(UAudioModuleStatics::GetBackgroundSoundVolume());
	SettingItem_EnvironmentSoundVolume->SetValue(UAudioModuleStatics::GetEnvironmentSoundVolume());
	SettingItem_EffectSoundVolume->SetValue(UAudioModuleStatics::GetEffectSoundVolume());
}

void UWidgetAudioSettingPage::OnApply()
{
	Super::OnApply();

	UAudioModuleStatics::SetGlobalSoundVolume(SettingItem_GlobalSoundVolume->GetValue().GetFloatValue());
	UAudioModuleStatics::SetBackgroundSoundVolume(SettingItem_BackgroundSoundVolume->GetValue().GetFloatValue());
	UAudioModuleStatics::SetEnvironmentSoundVolume(SettingItem_EnvironmentSoundVolume->GetValue().GetFloatValue());
	UAudioModuleStatics::SetEffectSoundVolume(SettingItem_EffectSoundVolume->GetValue().GetFloatValue());
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
	return UAudioModuleStatics::GetGlobalSoundVolume() != SettingItem_GlobalSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleStatics::GetBackgroundSoundVolume() != SettingItem_BackgroundSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleStatics::GetEnvironmentSoundVolume() != SettingItem_EnvironmentSoundVolume->GetValue().GetFloatValue() ||
		UAudioModuleStatics::GetEffectSoundVolume() != SettingItem_EffectSoundVolume->GetValue().GetFloatValue();
}

bool UWidgetAudioSettingPage::CanReset_Implementation() const
{
	return UAudioModuleStatics::GetGlobalSoundVolume() != GetDefaultAudioData().GlobalSoundParams.Volume ||
		UAudioModuleStatics::GetBackgroundSoundVolume() != GetDefaultAudioData().BackgroundSoundParams.Volume ||
		UAudioModuleStatics::GetEnvironmentSoundVolume() != GetDefaultAudioData().EnvironmentSoundParams.Volume ||
		UAudioModuleStatics::GetEffectSoundVolume() != GetDefaultAudioData().EffectSoundParams.Volume;
}

FDWAudioModuleSaveData& UWidgetAudioSettingPage::GetDefaultAudioData() const
{
	return USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().AudioData;
}
