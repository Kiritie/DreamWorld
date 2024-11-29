// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetVideoSettingPage.h"

#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

UWidgetVideoSettingPage::UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("视频"));
}

FSaveData* UWidgetVideoSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetOrCreateSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingModuleSaveData>().VideoData;
}
