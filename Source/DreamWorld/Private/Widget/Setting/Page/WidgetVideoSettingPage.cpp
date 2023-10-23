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

void UWidgetVideoSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetVideoSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	SettingItem_GlobalVideoQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality") }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GlobalVideoQuality, FText::FromString(TEXT("预设")));

	SettingItem_ViewDistanceQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ViewDistanceQuality, FText::FromString(TEXT("视距")));

	SettingItem_ShadowQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ShadowQuality, FText::FromString(TEXT("阴影")));

	SettingItem_GlobalIlluminationQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GlobalIlluminationQuality, FText::FromString(TEXT("光照")));

	SettingItem_ReflectionQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ReflectionQuality, FText::FromString(TEXT("反射")));

	SettingItem_AntiAliasingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_AntiAliasingQuality, FText::FromString(TEXT("抗锯齿")));

	SettingItem_TextureQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_TextureQuality, FText::FromString(TEXT("贴图")));

	SettingItem_VisualEffectQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_VisualEffectQuality, FText::FromString(TEXT("视效")));

	SettingItem_PostProcessingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_PostProcessingQuality, FText::FromString(TEXT("后处理")));

	SettingItem_FoliageQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_FoliageQuality, FText::FromString(TEXT("植被")));

	SettingItem_ShadingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ShadingQuality, FText::FromString(TEXT("遮蔽")));
}

void UWidgetVideoSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GlobalVideoQuality->SetValue((int32)ADWVideoModule::Get()->GetGlobalVideoQuality());
	SettingItem_ViewDistanceQuality->SetValue((int32)ADWVideoModule::Get()->GetViewDistanceQuality());
	SettingItem_ShadowQuality->SetValue((int32)ADWVideoModule::Get()->GetShadowQuality());
	SettingItem_GlobalIlluminationQuality->SetValue((int32)ADWVideoModule::Get()->GetGlobalIlluminationQuality());
	SettingItem_ReflectionQuality->SetValue((int32)ADWVideoModule::Get()->GetReflectionQuality());
	SettingItem_AntiAliasingQuality->SetValue((int32)ADWVideoModule::Get()->GetAntiAliasingQuality());
	SettingItem_TextureQuality->SetValue((int32)ADWVideoModule::Get()->GetTextureQuality());
	SettingItem_VisualEffectQuality->SetValue((int32)ADWVideoModule::Get()->GetVisualEffectQuality());
	SettingItem_PostProcessingQuality->SetValue((int32)ADWVideoModule::Get()->GetPostProcessingQuality());
	SettingItem_FoliageQuality->SetValue((int32)ADWVideoModule::Get()->GetFoliageQuality());
	SettingItem_ShadingQuality->SetValue((int32)ADWVideoModule::Get()->GetShadingQuality());
}

void UWidgetVideoSettingPage::OnApply()
{
	Super::OnApply();

	ADWVideoModule::Get()->SetGlobalVideoQuality((EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetViewDistanceQuality((EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetShadowQuality((EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetGlobalIlluminationQuality((EVideoQuality)SettingItem_GlobalIlluminationQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetReflectionQuality((EVideoQuality)SettingItem_ReflectionQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetAntiAliasingQuality((EVideoQuality)SettingItem_AntiAliasingQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetTextureQuality((EVideoQuality)SettingItem_TextureQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetVisualEffectQuality((EVideoQuality)SettingItem_VisualEffectQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetPostProcessingQuality((EVideoQuality)SettingItem_PostProcessingQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetFoliageQuality((EVideoQuality)SettingItem_FoliageQuality->GetValue().GetIntegerValue(), false);
	ADWVideoModule::Get()->SetShadingQuality((EVideoQuality)SettingItem_ShadingQuality->GetValue().GetIntegerValue(), false);

	ADWVideoModule::Get()->ApplyVideoQualitySettings();
}

void UWidgetVideoSettingPage::OnReset()
{
	Super::OnReset();

	SettingItem_GlobalVideoQuality->SetValue((int32)GetDefaultVideoData().GlobalVideoQuality);
	SettingItem_ViewDistanceQuality->SetValue((int32)GetDefaultVideoData().ViewDistanceQuality);
	SettingItem_ShadowQuality->SetValue((int32)GetDefaultVideoData().ShadowQuality);
	SettingItem_GlobalIlluminationQuality->SetValue((int32)GetDefaultVideoData().GlobalIlluminationQuality);
	SettingItem_ReflectionQuality->SetValue((int32)GetDefaultVideoData().ReflectionQuality);
	SettingItem_AntiAliasingQuality->SetValue((int32)GetDefaultVideoData().AntiAliasingQuality);
	SettingItem_TextureQuality->SetValue((int32)GetDefaultVideoData().TextureQuality);
	SettingItem_VisualEffectQuality->SetValue((int32)GetDefaultVideoData().VisualEffectQuality);
	SettingItem_PostProcessingQuality->SetValue((int32)GetDefaultVideoData().PostProcessingQuality);
	SettingItem_FoliageQuality->SetValue((int32)GetDefaultVideoData().FoliageQuality);
	SettingItem_ShadingQuality->SetValue((int32)GetDefaultVideoData().ShadingQuality);
}

void UWidgetVideoSettingPage::OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Super::OnValueChange(InSettingItem, InValue);

	if(InSettingItem == SettingItem_GlobalVideoQuality)
	{
		if(InValue != (int32)EVideoQuality::Custom)
		{
			SettingItem_ViewDistanceQuality->SetValue(InValue);
			SettingItem_ShadowQuality->SetValue(InValue);
			SettingItem_GlobalIlluminationQuality->SetValue(InValue);
			SettingItem_ReflectionQuality->SetValue(InValue);
			SettingItem_AntiAliasingQuality->SetValue(InValue);
			SettingItem_TextureQuality->SetValue(InValue);
			SettingItem_VisualEffectQuality->SetValue(InValue);
			SettingItem_PostProcessingQuality->SetValue(InValue);
			SettingItem_FoliageQuality->SetValue(InValue);
			SettingItem_ShadingQuality->SetValue(InValue);
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

void UWidgetVideoSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetVideoSettingPage::CanApply_Implementation() const
{
	return ADWVideoModule::Get()->GetGlobalVideoQuality() != (EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetViewDistanceQuality() != (EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetShadowQuality() != (EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetGlobalIlluminationQuality() != (EVideoQuality)SettingItem_GlobalIlluminationQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetReflectionQuality() != (EVideoQuality)SettingItem_ReflectionQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetAntiAliasingQuality() != (EVideoQuality)SettingItem_AntiAliasingQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetTextureQuality() != (EVideoQuality)SettingItem_TextureQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetVisualEffectQuality() != (EVideoQuality)SettingItem_VisualEffectQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetPostProcessingQuality() != (EVideoQuality)SettingItem_PostProcessingQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetFoliageQuality() != (EVideoQuality)SettingItem_FoliageQuality->GetValue().GetIntegerValue() ||
		ADWVideoModule::Get()->GetShadingQuality() != (EVideoQuality)SettingItem_ShadingQuality->GetValue().GetIntegerValue();
}

bool UWidgetVideoSettingPage::CanReset_Implementation() const
{
	return ADWVideoModule::Get()->GetGlobalVideoQuality() != GetDefaultVideoData().GlobalVideoQuality ||
		ADWVideoModule::Get()->GetViewDistanceQuality() != GetDefaultVideoData().ViewDistanceQuality ||
		ADWVideoModule::Get()->GetShadowQuality() != GetDefaultVideoData().ShadowQuality ||
		ADWVideoModule::Get()->GetGlobalIlluminationQuality() != GetDefaultVideoData().GlobalIlluminationQuality ||
		ADWVideoModule::Get()->GetReflectionQuality() != GetDefaultVideoData().ReflectionQuality ||
		ADWVideoModule::Get()->GetAntiAliasingQuality() != GetDefaultVideoData().AntiAliasingQuality ||
		ADWVideoModule::Get()->GetTextureQuality() != GetDefaultVideoData().TextureQuality ||
		ADWVideoModule::Get()->GetVisualEffectQuality() != GetDefaultVideoData().VisualEffectQuality ||
		ADWVideoModule::Get()->GetPostProcessingQuality() != GetDefaultVideoData().PostProcessingQuality ||
		ADWVideoModule::Get()->GetFoliageQuality() != GetDefaultVideoData().FoliageQuality ||
		ADWVideoModule::Get()->GetShadingQuality() != GetDefaultVideoData().ShadingQuality;
}

FDWVideoModuleSaveData& UWidgetVideoSettingPage::GetDefaultVideoData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().VideoData;
}
