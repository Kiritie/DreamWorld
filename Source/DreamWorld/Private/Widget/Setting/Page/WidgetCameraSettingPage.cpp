// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetCameraSettingPage.h"

#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

UWidgetCameraSettingPage::UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("摄像机"));
}

FSaveData* UWidgetCameraSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingModuleSaveData>().CameraData;
}
