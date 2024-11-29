// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetWidgetSettingPage.h"

#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

UWidgetWidgetSettingPage::UWidgetWidgetSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("界面"));
}

FSaveData* UWidgetWidgetSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetOrCreateSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingModuleSaveData>().WidgetData;
}
