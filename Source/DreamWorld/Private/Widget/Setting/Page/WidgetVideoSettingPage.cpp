// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetVideoSettingPage.h"

#include "Asset/AssetModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Video/DWVideoModule.h"
#include "Widget/WidgetModuleStatics.h"
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

	SettingItem_GlobalVideoQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality") }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GlobalVideoQuality, FText::FromString(TEXT("预设")));

	SettingItem_ViewDistanceQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ViewDistanceQuality, FText::FromString(TEXT("视距")));

	SettingItem_ShadowQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ShadowQuality, FText::FromString(TEXT("阴影")));

	SettingItem_GlobalIlluminationQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GlobalIlluminationQuality, FText::FromString(TEXT("光照")));

	SettingItem_ReflectionQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ReflectionQuality, FText::FromString(TEXT("反射")));

	SettingItem_AntiAliasingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_AntiAliasingQuality, FText::FromString(TEXT("抗锯齿")));

	SettingItem_TextureQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_TextureQuality, FText::FromString(TEXT("贴图")));

	SettingItem_VisualEffectQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_VisualEffectQuality, FText::FromString(TEXT("视效")));

	SettingItem_PostProcessingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_PostProcessingQuality, FText::FromString(TEXT("后处理")));

	SettingItem_FoliageQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_FoliageQuality, FText::FromString(TEXT("植被")));

	SettingItem_ShadingQuality = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("画质等级")), FString("/Script/WHFramework.EVideoQuality"), 5 }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_ShadingQuality, FText::FromString(TEXT("遮蔽")));
}

void UWidgetVideoSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GlobalVideoQuality->SetValue((int32)UDWVideoModule::Get().GetGlobalVideoQuality());
	SettingItem_ViewDistanceQuality->SetValue((int32)UDWVideoModule::Get().GetViewDistanceQuality());
	SettingItem_ShadowQuality->SetValue((int32)UDWVideoModule::Get().GetShadowQuality());
	SettingItem_GlobalIlluminationQuality->SetValue((int32)UDWVideoModule::Get().GetGlobalIlluminationQuality());
	SettingItem_ReflectionQuality->SetValue((int32)UDWVideoModule::Get().GetReflectionQuality());
	SettingItem_AntiAliasingQuality->SetValue((int32)UDWVideoModule::Get().GetAntiAliasingQuality());
	SettingItem_TextureQuality->SetValue((int32)UDWVideoModule::Get().GetTextureQuality());
	SettingItem_VisualEffectQuality->SetValue((int32)UDWVideoModule::Get().GetVisualEffectQuality());
	SettingItem_PostProcessingQuality->SetValue((int32)UDWVideoModule::Get().GetPostProcessingQuality());
	SettingItem_FoliageQuality->SetValue((int32)UDWVideoModule::Get().GetFoliageQuality());
	SettingItem_ShadingQuality->SetValue((int32)UDWVideoModule::Get().GetShadingQuality());
}

void UWidgetVideoSettingPage::OnApply()
{
	Super::OnApply();

	UDWVideoModule::Get().SetGlobalVideoQuality((EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetViewDistanceQuality((EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetShadowQuality((EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetGlobalIlluminationQuality((EVideoQuality)SettingItem_GlobalIlluminationQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetReflectionQuality((EVideoQuality)SettingItem_ReflectionQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetAntiAliasingQuality((EVideoQuality)SettingItem_AntiAliasingQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetTextureQuality((EVideoQuality)SettingItem_TextureQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetVisualEffectQuality((EVideoQuality)SettingItem_VisualEffectQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetPostProcessingQuality((EVideoQuality)SettingItem_PostProcessingQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetFoliageQuality((EVideoQuality)SettingItem_FoliageQuality->GetValue().GetIntegerValue(), false);
	UDWVideoModule::Get().SetShadingQuality((EVideoQuality)SettingItem_ShadingQuality->GetValue().GetIntegerValue(), false);

	UDWVideoModule::Get().ApplyVideoQualitySettings();
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
	return UDWVideoModule::Get().GetGlobalVideoQuality() != (EVideoQuality)SettingItem_GlobalVideoQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetViewDistanceQuality() != (EVideoQuality)SettingItem_ViewDistanceQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetShadowQuality() != (EVideoQuality)SettingItem_ShadowQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetGlobalIlluminationQuality() != (EVideoQuality)SettingItem_GlobalIlluminationQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetReflectionQuality() != (EVideoQuality)SettingItem_ReflectionQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetAntiAliasingQuality() != (EVideoQuality)SettingItem_AntiAliasingQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetTextureQuality() != (EVideoQuality)SettingItem_TextureQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetVisualEffectQuality() != (EVideoQuality)SettingItem_VisualEffectQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetPostProcessingQuality() != (EVideoQuality)SettingItem_PostProcessingQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetFoliageQuality() != (EVideoQuality)SettingItem_FoliageQuality->GetValue().GetIntegerValue() ||
		UDWVideoModule::Get().GetShadingQuality() != (EVideoQuality)SettingItem_ShadingQuality->GetValue().GetIntegerValue();
}

bool UWidgetVideoSettingPage::CanReset_Implementation() const
{
	return UDWVideoModule::Get().GetGlobalVideoQuality() != GetDefaultVideoData().GlobalVideoQuality ||
		UDWVideoModule::Get().GetViewDistanceQuality() != GetDefaultVideoData().ViewDistanceQuality ||
		UDWVideoModule::Get().GetShadowQuality() != GetDefaultVideoData().ShadowQuality ||
		UDWVideoModule::Get().GetGlobalIlluminationQuality() != GetDefaultVideoData().GlobalIlluminationQuality ||
		UDWVideoModule::Get().GetReflectionQuality() != GetDefaultVideoData().ReflectionQuality ||
		UDWVideoModule::Get().GetAntiAliasingQuality() != GetDefaultVideoData().AntiAliasingQuality ||
		UDWVideoModule::Get().GetTextureQuality() != GetDefaultVideoData().TextureQuality ||
		UDWVideoModule::Get().GetVisualEffectQuality() != GetDefaultVideoData().VisualEffectQuality ||
		UDWVideoModule::Get().GetPostProcessingQuality() != GetDefaultVideoData().PostProcessingQuality ||
		UDWVideoModule::Get().GetFoliageQuality() != GetDefaultVideoData().FoliageQuality ||
		UDWVideoModule::Get().GetShadingQuality() != GetDefaultVideoData().ShadingQuality;
}

FDWVideoModuleSaveData& UWidgetVideoSettingPage::GetDefaultVideoData() const
{
	return USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().VideoData;
}
