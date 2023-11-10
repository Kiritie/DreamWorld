// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetGameSettingPage.h"

#include "Asset/AssetModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Gameplay/DWGameMode.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleStatics.h"
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

	SettingItem_GameLevel = CreateSubWidget<UWidgetEnumSettingItem>({ FText::FromString(TEXT("游戏难度")), FString("/Script/DreamWorld.EDWGameLevel") }, UAssetModuleStatics::GetStaticClass(FName("EnumSettingItem")));
	AddSettingItem(SettingItem_GameLevel, FText::FromString(TEXT("游戏")));

	SettingItem_AutoJump = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("自动跳跃")) }, UAssetModuleStatics::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_AutoJump, FText::FromString(TEXT("控制")));
}

void UWidgetGameSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GameLevel->SetValue((int32)UCommonStatics::GetGameMode<ADWGameMode>()->GetGameLevel());
	SettingItem_AutoJump->SetValue(UCommonStatics::GetGameMode<ADWGameMode>()->IsAutoJump());
}

void UWidgetGameSettingPage::OnApply()
{
	Super::OnApply();

	UCommonStatics::GetGameMode<ADWGameMode>()->SetGameLevel((EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue());
	UCommonStatics::GetGameMode<ADWGameMode>()->SetAutoJump(SettingItem_AutoJump->GetValue().GetBooleanValue());
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
	return UCommonStatics::GetGameMode<ADWGameMode>()->GetGameLevel() != (EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue() ||
		UCommonStatics::GetGameMode<ADWGameMode>()->IsAutoJump() != SettingItem_AutoJump->GetValue().GetBooleanValue();
}

bool UWidgetGameSettingPage::CanReset_Implementation() const
{
	return UCommonStatics::GetGameMode<ADWGameMode>()->GetGameLevel() != GetDefaultGameData().GameLevel ||
		UCommonStatics::GetGameMode<ADWGameMode>()->IsAutoJump() != GetDefaultGameData().bAutoJump;
}

FDWGameSaveData& UWidgetGameSettingPage::GetDefaultGameData() const
{
	return USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().GameData;
}
