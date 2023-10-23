// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetGameSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetBoolSettingItem.h"
#include "Widget/Setting/Item/WidgetEnumSettingItem.h"

class UDWSettingSaveGame;

UWidgetGameSettingPage::UWidgetGameSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameSettingPage");

	Title = FText::FromString(TEXT("游戏"));
}

void UWidgetGameSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetGameSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	SettingItem_GameLevel = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("游戏难度")), FString("/Script/DreamWorld.EDWGameLevel") }, UAssetModuleBPLibrary::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GameLevel, FText::FromString(TEXT("游戏")));

	SettingItem_AutoJump = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("自动跳跃")) }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_AutoJump, FText::FromString(TEXT("控制")));
}

void UWidgetGameSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GameLevel->SetValue((int32)UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel());
	SettingItem_AutoJump->SetValue(UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump());
}

void UWidgetGameSettingPage::OnApply()
{
	Super::OnApply();

	UCommonBPLibrary::GetGameMode<ADWGameMode>()->SetGameLevel((EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue());
	UCommonBPLibrary::GetGameMode<ADWGameMode>()->SetAutoJump(SettingItem_AutoJump->GetValue().GetBooleanValue());
}

void UWidgetGameSettingPage::OnReset()
{
	Super::OnReset();

	SettingItem_GameLevel->SetValue((int32)GetDefaultGameData().GameLevel);
	SettingItem_AutoJump->SetValue(GetDefaultGameData().bAutoJump);
}

void UWidgetGameSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetGameSettingPage::CanApply_Implementation() const
{
	return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != (EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue() ||
		UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump() != SettingItem_AutoJump->GetValue().GetBooleanValue();
}

bool UWidgetGameSettingPage::CanReset_Implementation() const
{
	return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != GetDefaultGameData().GameLevel ||
		UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump() != GetDefaultGameData().bAutoJump;
}

FDWGameSaveData& UWidgetGameSettingPage::GetDefaultGameData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().GameData;
}
