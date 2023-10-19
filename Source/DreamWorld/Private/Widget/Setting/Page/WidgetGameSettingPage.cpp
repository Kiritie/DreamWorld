// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetGameSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetBoolSettingItem.h"
#include "Widget/Setting/Item/WidgetEnumSettingItem.h"

UWidgetGameSettingPage::UWidgetGameSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameSettingPage");

	Title = FText::FromString(TEXT("游戏"));
	DefaultGameData = FDWGameSaveData();
}

void UWidgetGameSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetGameSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	SettingItem_GameLevel = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetEnumSettingItem>({ FText::FromString(TEXT("游戏难度")), FString("/Script/DreamWorld.EDWGameLevel") }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GameLevel, FText::FromString(TEXT("游戏")));

	SettingItem_AutoJump = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetBoolSettingItem>({ FText::FromString(TEXT("自动跳跃")), FString("/Script/DreamWorld.EDWGameLevel") }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_AutoJump, FText::FromString(TEXT("控制")));
}

void UWidgetGameSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	SettingItem_GameLevel->SetValue((int32)UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel());
	SettingItem_AutoJump->SetValue(UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump());
}

void UWidgetGameSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();

	UCommonBPLibrary::GetGameMode<ADWGameMode>()->SetGameLevel((EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue());
	UCommonBPLibrary::GetGameMode<ADWGameMode>()->SetAutoJump(SettingItem_AutoJump->GetValue().GetBooleanValue());
}

void UWidgetGameSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();

	SettingItem_GameLevel->SetValue((int32)DefaultGameData.GameLevel);
	SettingItem_AutoJump->SetValue(DefaultGameData.bAutoJump);
}

void UWidgetGameSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetGameSettingPage::CanApply_Implementation() const
{
	return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != (EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue() ||
		UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump() != SettingItem_AutoJump->GetValue().GetBooleanValue();
}

bool UWidgetGameSettingPage::CanReset_Implementation() const
{
	return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != DefaultGameData.GameLevel ||
		UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump() != DefaultGameData.bAutoJump;
}
