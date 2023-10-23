// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetInputSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Common/CommonBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetKeySettingItem.h"

class UDWSettingSaveGame;

UWidgetInputSettingPage::UWidgetInputSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameSettingPage");

	Title = FText::FromString(TEXT("游戏"));
}

void UWidgetInputSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetInputSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	SettingItem_Jump = CreateSubWidget<UWidgetKeySettingItem>({ FText::FromString(TEXT("跳跃")) }, UAssetModuleBPLibrary::GetStaticClass(FName("KeySettingItem")));
	AddSettingItem(SettingItem_Jump, FText::FromString(TEXT("角色")));
}

void UWidgetInputSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_Jump->SetValue((int32)UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel());
}

void UWidgetInputSettingPage::OnApply()
{
	Super::OnApply();

	UCommonBPLibrary::GetGameMode<ADWGameMode>()->SetGameLevel((EDWGameLevel)SettingItem_Jump->GetValue().GetIntegerValue());
}

void UWidgetInputSettingPage::OnReset()
{
	Super::OnReset();

	// SettingItem_Jump->SetValue((int32)GetDefaultInputData().GameLevel);
}

void UWidgetInputSettingPage::OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Super::OnValueChange(InSettingItem, InValue);
}

void UWidgetInputSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetInputSettingPage::CanApply_Implementation() const
{
	return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != (EDWGameLevel)SettingItem_Jump->GetValue().GetIntegerValue();
}

bool UWidgetInputSettingPage::CanReset_Implementation() const
{
	return false;
	// return UCommonBPLibrary::GetGameMode<ADWGameMode>()->GetGameLevel() != GetDefaultInputData().GameLevel ||
	// 	UCommonBPLibrary::GetGameMode<ADWGameMode>()->IsAutoJump() != GetDefaultInputData().bAutoJump;
}

FDWInputModuleSaveData& UWidgetInputSettingPage::GetDefaultInputData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().InputData;
}
