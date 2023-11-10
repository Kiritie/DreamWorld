// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetAudioSettingPage.h"

#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"

UWidgetAudioSettingPage::UWidgetAudioSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("音频"));
}

FSaveData* UWidgetAudioSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().AudioData;
}
