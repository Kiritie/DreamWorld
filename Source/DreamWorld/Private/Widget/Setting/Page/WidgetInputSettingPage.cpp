// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetInputSettingPage.h"

#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

UWidgetInputSettingPage::UWidgetInputSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("输入"));
}

FSaveData* UWidgetInputSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().InputData;
}
