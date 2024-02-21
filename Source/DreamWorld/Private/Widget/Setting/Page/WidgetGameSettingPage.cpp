// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetGameSettingPage.h"

#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Setting/DWSettingModule.h"
#include "Setting/SettingModule.h"
#include "Setting/Widget/Item/WidgetBoolSettingItemBase.h"
#include "Setting/Widget/Item/WidgetEnumSettingItemBase.h"

UWidgetGameSettingPage::UWidgetGameSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameSettingPage");

	Title = FText::FromString(TEXT("游戏"));
}

void UWidgetGameSettingPage::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetGameSettingPage::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	SettingItem_GameLevel = UObjectPoolModuleStatics::SpawnObject<UWidgetEnumSettingItemBase>(nullptr, { FText::FromString(TEXT("游戏难度")), FString("/Script/DreamWorld.EDWGameLevel") }, USettingModule::Get().GetEnumSettingItemClass());
	AddSettingItem(FName("GameLevel"), SettingItem_GameLevel, FText::FromString(TEXT("游戏")));

	SettingItem_AutoJump = UObjectPoolModuleStatics::SpawnObject<UWidgetBoolSettingItemBase>(nullptr, { FText::FromString(TEXT("自动跳跃")) }, USettingModule::Get().GetBoolSettingItemClass());
	AddSettingItem(FName("AutoJump"), SettingItem_AutoJump, FText::FromString(TEXT("控制")));
}

void UWidgetGameSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_GameLevel->SetValue((int32)UDWSettingModule::Get().GetGameLevel());
	SettingItem_AutoJump->SetValue(UDWSettingModule::Get().IsAutoJump());
}

void UWidgetGameSettingPage::OnApply()
{
	Super::OnApply();

	UDWSettingModule::Get().SetGameLevel((EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue());
	UDWSettingModule::Get().SetAutoJump(SettingItem_AutoJump->GetValue().GetBooleanValue());
}

void UWidgetGameSettingPage::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	SettingItem_GameLevel->SetValue((int32)GetDefaultSaveData()->CastRef<FDWGameSaveData>().GameLevel);
	SettingItem_AutoJump->SetValue(GetDefaultSaveData()->CastRef<FDWGameSaveData>().bAutoJump);
}

void UWidgetGameSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetGameSettingPage::CanApply_Implementation() const
{
	return UDWSettingModule::Get().GetGameLevel() != (EDWGameLevel)SettingItem_GameLevel->GetValue().GetIntegerValue() ||
		UDWSettingModule::Get().IsAutoJump() != SettingItem_AutoJump->GetValue().GetBooleanValue();
}

bool UWidgetGameSettingPage::CanReset_Implementation() const
{
	return UDWSettingModule::Get().GetGameLevel() != GetDefaultSaveData()->CastRef<FDWGameSaveData>().GameLevel ||
		UDWSettingModule::Get().IsAutoJump() != GetDefaultSaveData()->CastRef<FDWGameSaveData>().bAutoJump;
}

FSaveData* UWidgetGameSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingModuleSaveData>().GameData;
}
